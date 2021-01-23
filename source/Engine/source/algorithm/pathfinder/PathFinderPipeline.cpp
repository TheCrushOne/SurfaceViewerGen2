#include "stdafx.h"
#include "PathFinderPipeline.h"
#include <math.h>
#include <limits.h>
#include "Helpers\CoordinateCorrectionHelper.h"

using namespace SV;
using namespace SV::pathfinder;

PathFinderPipeline::PathFinderPipeline(central_pack* pack)
//: m_data(LightPointData())
//: m_pointScore(0)
   : m_rowCount(0)
   , m_colCount(0)
   , m_statistic(nullptr)
   , m_coverageBuilder(std::make_unique<CoverageBuilder>())
   , m_strategyManager(std::make_unique<StrategyManager>(pack))
   , m_routeLinePreparer(std::make_unique<RouteLinePreparer>(pack))
   , m_threadSplitter(std::make_unique<ThreadSplitter>(pack))
   //, m_packetMutex(m_taskPacket)
   , Central(pack)
{}

PathFinderPipeline::~PathFinderPipeline()
{}

// NOTE: желательно и лаунчер запускать в своем потоке
// WARNING: распараллелено!!!
void PathFinderPipeline::FindPath(std::function<void(void)> callback, std::shared_ptr<settings::application_settings> settings, const SharedRoutePointMatrix rawdata, std::shared_ptr<path_finder_indata> indata)
{
   // NOTE: итерация шага стратегий
   m_iterations = 2;

   m_callback = callback;
   m_pathFound = false;
   m_rawdata = rawdata;
   m_indata = indata;
   m_rowCount = rawdata->GetRowCount();
   m_colCount = rawdata->GetColCount();
   m_settings = settings;

   // NOTE: вот на всякий очистить надо..
   m_coverageHistory.clear();
   m_threadSplitter->SetSettings(m_indata->settings);
   m_routeLinePreparer->SetCoverageMatrix(m_currentCoverage);
   m_routeLinePreparer->SetRawDataMatrix(rawdata);

   TaskHolder::FixCurrentTime();

   findPathMultiThread();
   /*if (m_indata->settings.multithread)
      findPathMultiThread();
   else
      findPathSingleThread();*/
}

void PathFinderPipeline::prepareSourcePoints()
{
   m_paths.air_routes.clear();
   m_paths.air_routes.resize(m_indata->unit_data.air_units.size());
   m_paths.land_routes.clear();
   m_paths.land_routes.resize(m_indata->unit_data.land_units.size());
   for (size_t idx = 0; idx < m_indata->unit_data.air_units.size(); idx++)
   {
      // NOTE: тут не должно быть к-т, т.к. "литаки" на автораспределении точек по стратегиям
      ATLASSERT(!m_indata->settings.use_strategies || m_indata->unit_data.air_units.at(idx).control_point_list.empty());
      m_paths.air_routes.at(idx) = m_indata->unit_data.air_units.at(idx);
   }
   for (size_t idx = 0; idx < m_indata->unit_data.land_units.size(); idx++)
      m_paths.land_routes.at(idx) = m_indata->unit_data.land_units.at(idx);
}

void PathFinderPipeline::findPathMultiThread()
{
   // TODO: приколхозить ThreadJobManager из юнидаты...хотя...

   pipelineStep();  // NOTE: стратегии
}

void PathFinderPipeline::pipelineStep()
{
   generateIterationStep();
   if (!m_currentCoverage.get())
      m_currentCoverage = std::move(m_coverageBuilder->BuildLandCoverage(m_rowCount, m_colCount, m_indata->strategy_settings, m_paths.air_routes));
   m_routeLinePreparer->SetCoverageMatrix(m_currentCoverage);
   generatePathfinderTaskList(true);
   m_threadSplitter->CountCurrent(m_routeLinePreparer->GetCurrentTaskList(), [this]() { onAirPathsComputed(); });
}

void PathFinderPipeline::generateIterationStep()
{
   // NOTE: распределение точек в соответствии со стратегией
   prepareSourcePoints();
   if (m_indata->settings.use_strategies)
      m_strategyManager->PrepareControlPoint(m_iterations, m_paths.land_routes, m_paths.air_routes, m_rawdata, m_indata);
}

void PathFinderPipeline::generatePathfinderTaskList(bool isAir)
{
   m_routeLinePreparer->PrepareTaskList(isAir ? m_paths.air_routes : m_paths.land_routes, isAir);
}

void PathFinderPipeline::buildLandCoverage()
{
   if (m_indata->settings.land_path)
   {
      if (updateCurrentCoverage())
      {
         correctControlPoints();
         findLandRoute();
      }
      else
      {
         // NOTE: покрытия совпали, дроны исследовали всё
         m_pathFound = false;
         pipelineFinalize();
      }
   }
   else
   {
      m_pathFound = true;
      pipelineFinalize();
   }
   GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "Land coverage complete");
}

// NOTE: сравнение старой матрицы покрытия с новой
bool PathFinderPipeline::checkLandCoverage(const SharedUnsignedMatrix& coverageMatrix)
{
   ATLASSERT(coverageMatrix->GetRowCount() == m_currentCoverage->GetRowCount());
   ATLASSERT(coverageMatrix->GetColCount() == m_currentCoverage->GetColCount());
   for (size_t rowIdx = 0; rowIdx < coverageMatrix->GetRowCount(); rowIdx++)
   {
      for (size_t colIdx = 0; colIdx < coverageMatrix->GetColCount(); colIdx++)
      {
         if (coverageMatrix->Get(rowIdx, colIdx) != m_currentCoverage->Get(rowIdx, colIdx))
            return true;
      }
   }
   return false;
}

void PathFinderPipeline::findLandRoute()
{
   // NOTE: наземный пока что один, так что просто считаем его в потоке
   ATLASSERT(m_indata->unit_data.land_units.size() == 1);
   generatePathfinderTaskList(false);
   m_threadSplitter->CountCurrent(m_routeLinePreparer->GetCurrentTaskList(), [this]() { onLandPathsComputed(); });
}

void PathFinderPipeline::restorePathList(bool isAir)
{
   m_routeLinePreparer->RestorePathList(isAir ? m_paths.air_routes : m_paths.land_routes, isAir);
}

void PathFinderPipeline::onAirPathsComputed()
{
   restorePathList(true);
   buildLandCoverage();
}

void PathFinderPipeline::onLandPathsComputed()
{
   restorePathList(false);
   auto taskList = m_routeLinePreparer->GetCurrentTaskList();
   m_pathFound = taskList.at(0).path_found;
   if (m_pathFound)
      pipelineFinalize();
   else
      pipelineStep();
}

void PathFinderPipeline::pipelineFinalize()
{
   m_callback();
}

bool PathFinderPipeline::updateCurrentCoverage()
{
   ATLASSERT(m_currentCoverage.get());
   auto newCoverage = m_coverageBuilder->BuildLandCoverage(m_rowCount, m_colCount, m_indata->strategy_settings, m_paths.air_routes);
   if (checkLandCoverage(newCoverage))
   {
      m_currentCoverage = std::move(newCoverage);
      m_coverageHistory.emplace_back(m_currentCoverage);
      return true;
   }
   else
      m_coverageHistory.emplace_back(std::move(newCoverage));
   return false;
}

void PathFinderPipeline::correctControlPoints()
{
   affilationCheckerMtd affilationChecker = [this](const std::shared_ptr<pathfinder::RoutePointMatrix>& rawdata, size_t row, size_t col)->bool
   {
      return this->GetCurrentCoverage()->Get(row, col) == 1
         && rawdata->Get(row, col).go != pathfinder::GoZoneAffilation::GZA_FORBIDDEN;
   };
   auto corrector = [this, affilationChecker](CG::route_point& src)
   {
      CG::route_point corrected;
      corrected = CoordinateCorrectionHelper::CorrectPoint(m_rawdata, src.row, src.col, affilationChecker, GetCommunicator());
      src = corrected;
   };
   auto& landUnitData = m_paths.land_routes.at(0);
   corrector(landUnitData.start);
   corrector(landUnitData.finish);
   for (auto& elem : landUnitData.control_point_list)
      corrector(elem);
}