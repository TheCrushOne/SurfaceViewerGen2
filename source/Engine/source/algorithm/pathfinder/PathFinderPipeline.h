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
#include "Multithread/MCManager.h"
#include "PathFinder.h"
#include "common/central_class.h"

namespace SV::pathfinder
{
   class PathFinderPipeline : public Central
   {
   public:
      PathFinderPipeline(central_pack* pack);
   public:
      ~PathFinderPipeline();
   public:
      void FindPath(std::function<void(void)> callback, const std::shared_ptr<RoutePointMatrix> rawdata, std::shared_ptr<path_finder_indata> indata);
      const pathfinder::route_data& GetPaths() const { return m_paths; }
      const std::shared_ptr<Matrix<size_t>>& GetCurrentCoverage() const { return m_currentCoverage; }
   private:
      void prepareSourcePoints();
      void correctControlPoints();

      void findPathMultiThread();
      void findPathSingleThread();

      void pipelineStep();

      void generateIterationStep();
      void formatTaskPool();
      void formatTaskPacket();
      //void onAirRouteTaskHolderFinished();
      void onAirRoutePacketFinished();
      void buildLandCoverage();
      bool checkLandCoverage(std::shared_ptr<Matrix<size_t>> coverageMatrix);
      void findLandRoute();
      void pipelineFinalize();
   private:
      std::function<void(void)> m_callback;
      std::unique_ptr<PathFinder> m_pathfinder;

      std::shared_ptr<RoutePointMatrix> m_rawdata;
      std::shared_ptr<path_finder_indata> m_indata;

      std::shared_ptr<Matrix<size_t>> m_currentCoverage;
      

      TaskStorage m_taskPool;
      std::shared_ptr<TaskStorage> m_taskPacket;
      //std::mutex m_packetMutex;

      route_data m_paths;

      std::shared_ptr<settings::application_settings> m_appSettings;

      bool m_pathFound = false;
      size_t m_iterations = 0;

      // NOTE: Опасно!!!
      path_finder_statistic* m_statistic;

      std::unique_ptr<CoverageBuilder> m_coverageBuilder;
      std::unique_ptr<StrategyManager> m_strategyManager;
      std::unique_ptr<MultithreadComputingManager> m_taskManager;

      //ExperimentMeta m_vmeta;
      size_t m_rowCount;
      size_t m_colCount;
   };
}