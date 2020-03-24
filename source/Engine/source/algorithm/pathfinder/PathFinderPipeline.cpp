#include "stdafx.h"
#include "PathFinderPipeline.h"
#include <math.h>
#include <limits.h>

using namespace pathfinder;

PathFinderPipeline::PathFinderPipeline()
//: m_data(LightPointData())
//: m_pointScore(0)
   : m_rowCount(0)
   , m_colCount(0)
   , m_statistic(nullptr)
   , m_coverageBuilder(std::make_unique<CoverageBuilder>())
   , m_strategyManager(std::make_unique<StrategyManager>())
   , m_pathfinder(std::make_unique<PathFinder>())
   , m_taskPacket(std::make_shared<std::vector<pathfinder::task_unit>>())
{}

PathFinderPipeline::~PathFinderPipeline()
{}

// NOTE: желательно и лаунчер запускать в своем потоке
// WARNING: распараллелено!!!
void PathFinderPipeline::FindPath(std::function<void(void)> callback, const std::shared_ptr<Matrix<SVCG::route_point>> rawdata, std::shared_ptr<path_finder_indata> indata)
{
   //m_statistic = &statistic;
   //m_vmeta = ExperimentMeta{
   //   pathFinderSettings.statFieldIndex,
   //   pathFinderSettings.multithread
   //};
   // NOTE: итерация шага стратегий
   m_iterations = 2;

   m_callback = callback;
   m_pathFound = false;
   m_rawdata = rawdata;
   m_indata = indata;
   //qint64 startTime = CURTIME_MS();
   m_rowCount = rawdata->GetRowCount();
   m_colCount = rawdata->GetColCount();
   //m_funLandVector.clear();
   m_appSettings = std::make_shared<settings::application_settings>();//WFM::GetSharedInstance<Dispatcher>(DBG_DATA)->GetSettings();

   if (m_indata->settings.multithread)
      findPathMultiThread();
   else
      findPathSingleThread();
   //qint64 finishTime = CURTIME_MS();
   //qint64 time = finishTime - startTime;
   //qDebug() << "path finding ended: mt: " << pathFinderSettings.multithread << " :" << time;
   //TaskStat stat{m_vmeta, time};
   //QVariant vdata;
   //vdata.setValue(stat);
   //WFM::GetSharedInstance<Statistic>(DBG_DATA)->StatUpdateTaskRun(vdata);
   /*if (m_appSettings->type == STT::ApplicationRunType::ART_RESEARCH
       && m_appSettings->res_settings.type == STT::ResearchType::RT_GEN1)
      emit TaskDataUpdate(vdata);*/
      //clearSupportData();
   //statistic = *m_statistic;
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

void PathFinderPipeline::onAirRouteTaskHolderFinished()
{
   for (const auto& task : *m_taskPacket.get())
   {
      if (task.status != TaskStatus::TS_FINISHED)
         return;
   }
   onAirRoutePacketFinished();
}

void PathFinderPipeline::onAirRoutePacketFinished()
{
   //m_communicator->Message(ICommunicator::MessageType::MS_Error, "air route packet finished");
   static const unsigned long long int threadCountSpec = std::thread::hardware_concurrency();
   if (m_taskPool.size() == 0)
   {
      buildLandCoverage();
      return;
   }
   formatTaskPacket();

   // test 4/8
   const int threadCount = 16;   // NOTE: По количеству логических ядер 8+HT
   m_holders.clear();
   m_holders.resize(threadCount);   // TODO: чекнуть, вызывается ли конструктор
   //QThreadPool::globalInstance()->setMaxThreadCount(threadCount);
   Message(ICommunicator::MS_Debug, "ar task packet finished %i", m_taskPool.size());
   for (auto& holder : m_holders)
   {
      holder.first.Launch(m_taskPacket,
         [this]()
         {
            onAirRouteTaskHolderFinished();
         });
   }
}

void PathFinderPipeline::buildLandCoverage()
{
   if (m_indata->settings.land_path)
   {
      auto newCoverage = m_coverageBuilder->BuildLandCoverage(m_rowCount, m_colCount, m_indata->strategy_settings, m_paths.air_routes);
      if (!m_currentCoverage.get() || checkLandCoverage(newCoverage))
      {
         m_currentCoverage = std::move(newCoverage);
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