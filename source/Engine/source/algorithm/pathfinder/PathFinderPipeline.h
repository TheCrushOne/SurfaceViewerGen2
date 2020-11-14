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
      void FindPath(std::function<void(void)> callback, std::shared_ptr<settings::application_settings>, const SharedRoutePointMatrix rawdata, std::shared_ptr<path_finder_indata> indata);
      const pathfinder::route_data& GetPaths() const { return m_paths; }
      const SharedUnsignedMatrix& GetCurrentCoverage() const { return m_currentCoverage; }
      const std::vector<SharedUnsignedMatrix>& GetCoverageHistory() const { return m_coverageHistory; }
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
      bool checkLandCoverage(const SharedUnsignedMatrix& coverageMatrix);
      void findLandRoute();
      void pipelineFinalize();
      bool updateCurrentCoverage();
   private:
      std::function<void(void)> m_callback;
      std::unique_ptr<PathFinder> m_pathfinder;

      std::shared_ptr<RoutePointMatrix> m_rawdata;
      std::shared_ptr<path_finder_indata> m_indata;

      SharedUnsignedMatrix m_currentCoverage;
      std::vector<SharedUnsignedMatrix> m_coverageHistory;

      TaskStorage m_taskPool;
      std::shared_ptr<TaskStorage> m_taskPacket;
      //std::mutex m_packetMutex;

      route_data m_paths;

      std::shared_ptr<settings::application_settings> m_settings;

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