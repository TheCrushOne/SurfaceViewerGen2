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
   , m_taskManager(std::make_unique<MultithreadComputingManager>(pack))
   , m_pathfinder(std::make_unique<PathFinder>())
   //, m_packetMutex(m_taskPacket)
   , m_taskPacket(std::make_shared<TaskStorage>())
   , Central(pack)
{
   // TODO: check!!!
   m_taskManager->SetTaskPacketFinishCallback([this] { onAirRoutePacketFinished(); });
}

PathFinderPipeline::~PathFinderPipeline()
{}

// NOTE: желательно и лаунчер запускать в своем потоке
// WARNING: распараллелено!!!
void PathFinderPipeline::FindPath(std::function<void(void)> callback, const std::shared_ptr<RoutePointMatrix> rawdata, std::shared_ptr<path_finder_indata> indata)
{
   // NOTE: итерация шага стратегий
   m_iterations = 2;

   m_callback = callback;
   m_pathFound = false;
   m_rawdata = rawdata;
   m_indata = indata;
   m_rowCount = rawdata->GetRowCount();
   m_colCount = rawdata->GetColCount();
   m_appSettings = std::make_shared<settings::application_settings>();

   if (m_indata->settings.multithread)
      findPathMultiThread();
   else
      findPathSingleThread();
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
      ATLASSERT(m_indata->unit_data.air_units.at(idx).control_point_list.empty());
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
   formatTaskPool();
   onAirRoutePacketFinished();
}

void PathFinderPipeline::generateIterationStep()
{
   // NOTE: распределение точек в соответствии со стратегией
   prepareSourcePoints();
   if (m_indata->settings.use_strategies)
      m_strategyManager->PrepareControlPoint(m_iterations, m_paths.land_routes, m_paths.air_routes, m_rawdata, m_indata);
}

void PathFinderPipeline::formatTaskPool()
{
   m_taskPool.clear();
   m_taskPool.resize(m_indata->unit_data.air_units.size());

   for (size_t idx = 0; idx < m_taskPool.size(); idx++)
   {
      auto& path = m_paths.air_routes.at(idx);
      m_taskPool.at(idx).status = TaskStatus::TS_QUEUED;
      m_taskPool.at(idx).index = idx;
      m_taskPool.at(idx).runnable = [this, &path]()
      {
         this->m_pathfinder->FindAirPath(path, m_rawdata, m_iterations, true);
      };
   }
}

void PathFinderPipeline::formatTaskPacket()
{
   m_taskPacket->clear();
   for (size_t idx = 0; idx < m_indata->settings.packet_size && m_taskPool.size() > 0; idx++)
   {
      m_taskPacket->emplace_back(m_taskPool.back());
      m_taskPool.pop_back();
   }
}

//void PathFinderPipeline::onAirRouteTaskHolderFinished()
//{
//   for (const auto& task : *m_taskPacket.get())
//   {
//      if (task.status != TaskStatus::TS_FINISHED)
//         return;
//   }
//   onAirRoutePacketFinished();
//}

void PathFinderPipeline::onAirRoutePacketFinished()
{
   static const unsigned long long int threadCountSpec = std::thread::hardware_concurrency();

   if (m_taskPool.size() == 0)
   {
      m_taskManager->Finale();
      buildLandCoverage();
      GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "Land coverage complete");
      return;
   }
   formatTaskPacket();

   // test 4/8
   //const int threadCount = 16;   // NOTE: По количеству логических ядер 8+HT
   m_taskManager->SetHolderCount(m_indata->settings.thread_count);
   m_taskManager->LaunchTaskPacket(m_taskPacket);
}

void PathFinderPipeline::buildLandCoverage()
{
   if (m_indata->settings.land_path)
   {
      auto newCoverage = m_coverageBuilder->BuildLandCoverage(m_rowCount, m_colCount, m_indata->strategy_settings, m_paths.air_routes);
      if (!m_currentCoverage.get() || checkLandCoverage(newCoverage))
      {
         m_currentCoverage = std::move(newCoverage);
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
}

bool PathFinderPipeline::checkLandCoverage(std::shared_ptr<Matrix<size_t>> coverageMatrix)
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
   m_pathfinder->FindLandPath(m_paths.land_routes.at(0), m_rawdata, m_currentCoverage, true, &m_pathFound);
   if (m_pathFound)
      pipelineFinalize();
   else
      pipelineStep();
}

void PathFinderPipeline::pipelineFinalize()
{
   m_callback();
}

void PathFinderPipeline::findPathSingleThread()
{
   do
   {
      m_paths.air_routes.clear();
      m_paths.air_routes.resize(m_indata->unit_data.air_units.size());
      if (m_indata->settings.use_strategies)
         m_strategyManager->PrepareControlPoint(m_iterations, m_paths.land_routes, m_paths.air_routes, m_rawdata, m_indata);
      for (size_t idx = 0; idx < m_paths.air_routes.size(); idx++)
         m_pathfinder->FindAirPath(m_paths.air_routes.at(idx), m_rawdata, m_iterations, m_indata->settings.multithread);

      if (m_indata->settings.land_path)
      {
         auto newCoverage = m_coverageBuilder->BuildLandCoverage(m_rowCount, m_colCount, m_indata->strategy_settings, m_paths.air_routes);
         if (!m_currentCoverage.get() || checkLandCoverage(newCoverage))
         {
            m_currentCoverage = std::move(newCoverage);

            ATLASSERT(m_indata->unit_data.land_units.size() == 1);
            m_paths.land_routes.clear();
            m_paths.land_routes.resize(1);
            m_pathfinder->FindLandPath(m_paths.land_routes.at(0), m_rawdata, m_currentCoverage, false, &m_pathFound);
            if (m_pathFound)
            {
               pipelineFinalize();
               return;
            }
         }
         else
         {
            // NOTE: покрытия совпали, дроны исследовали всё
            m_pathFound = false;
            pipelineFinalize();
            return;
         }
      }
      else
         m_pathFound = true;

      //qDebug() << pathFound << iterations;
      m_iterations++;
   } while (!m_pathFound);
}

void PathFinderPipeline::correctControlPoints()
{
   affilationCheckerMtd affilationChecker = [this](const std::shared_ptr<pathfinder::RoutePointMatrix>& rawdata, size_t row, size_t col)->bool
   {
      return this->GetCurrentCoverage()->Get(row, col) == 1 && rawdata->Get(row, col).go != pathfinder::GoZoneAffilation::GZA_FORBIDDEN;
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