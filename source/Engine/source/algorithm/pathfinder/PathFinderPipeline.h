#pragma once

#include <memory>
#include <functional>
#include <iostream>
#include <algorithm>
#include <future>
#include "common/coordinates.h"
#include "SVCG/route_point.h"
#include "common/pathfinder_types.h"
#include "CoverageBuilder.h"
#include "StrategyManager.h"
#include "TaskHolder/TaskHolder.h"
#include "PathFinder.h"

namespace pathfinder
{
   class PathFinderPipeline
   {
   public:
      PathFinderPipeline();
   public:
      ~PathFinderPipeline();
   public:
      void FindPath(std::function<void(void)> callback, const std::shared_ptr<Matrix<SVCG::route_point>> rawdata, std::shared_ptr<path_finder_indata>/*bool multithread = true, size_t countIdx = 0, size_t legnthIdx = 0, bool research = false, bool landPath = true, size_t packetSize = 0*//*strategy_settings settings, const path_finder_settings pathFinderSettings*//*, path_finder_statistic& statistic*/);
      const pathfinder::route_data& GetPaths() const { return m_paths; }
   private:
      void prepareSourcePoints();

      void findPathMultiThread();
      void findPathSingleThread();

      void pipelineStep();

      void generateIterationStep();
      void formatTaskPool();
      void formatTaskPacket();
      void onAirRouteTaskHolderFinished();
      void onAirRoutePacketFinished();
      void buildLandCoverage();
      bool checkLandCoverage(std::shared_ptr<Matrix<size_t>> coverageMatrix);
      void findLandRoute();
      void pipelineFinalize();
   private:
      std::function<void(void)> m_callback;
      std::unique_ptr<PathFinder> m_pathfinder;

      std::shared_ptr<Matrix<SVCG::route_point>> m_rawdata;
      std::shared_ptr<path_finder_indata> m_indata;

      std::shared_ptr<Matrix<size_t>> m_currentCoverage;
      std::vector<std::pair<TaskHolder, TaskStatus>> m_holders;

      std::vector<task_unit> m_taskPool;
      std::shared_ptr<std::vector<task_unit>> m_taskPacket;

      route_data m_paths;

      std::shared_ptr<settings::application_settings> m_appSettings;

      bool m_pathFound;
      size_t m_iterations;

      // NOTE: Опасно!!!
      path_finder_statistic* m_statistic;

      std::unique_ptr<CoverageBuilder> m_coverageBuilder;
      std::unique_ptr<StrategyManager> m_strategyManager;

      //ExperimentMeta m_vmeta;
      size_t m_rowCount;
      size_t m_colCount;
   };
}