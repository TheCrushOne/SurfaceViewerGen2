#pragma once

//#include "base/instance.h"
//#include "main/engine.h"
#include <memory>
#include "algorithm/statistic.h"
#include <functional>
#include <iostream>
#include <algorithm>
#include "common/coordinates.h"
#include "SVCG/route_point.h"
#include "common/pathfinder_types.h"
#include <future>
#include "CoverageBuilder.h"
#include "StrategyManager.h"
#include "TaskHolder/TaskHolder.h"

namespace pathfinder
{
   class PathFinder
   {
      //INST_OBJECT(PathFinder)
   public:
      PathFinder();
   public:
      ~PathFinder();
   protected:
      /*virtual void initPowertrain() override;
      virtual void initMiscellaneous() override;*/
   public:
      route_data FindPath(std::function<void(void)> callback, const std::shared_ptr<Matrix<SVCG::route_point>> rawdata, /*bool multithread = true, size_t countIdx = 0, size_t legnthIdx = 0, bool research = false, bool landPath = true, size_t packetSize = 0*/strategy_settings settings, const path_finder_settings pathFinderSettings/*, path_finder_statistic& statistic*/);
   private:
      route findLandPath(route& route, const std::shared_ptr<Matrix<SVCG::route_point>> rawdata, std::shared_ptr<Matrix<size_t>> coverageMatrix, bool multithread, bool* pathFounded);
      route findAirPath(route& route, const std::shared_ptr<Matrix<SVCG::route_point>> rawdata, size_t iterations, bool multithread);
      std::vector<SVCG::route_point> findPath(const std::shared_ptr<Matrix<SVCG::route_point>> rawdata, SVCG::route_point& start, SVCG::route_point& finish, path_finder_logic& logic, std::shared_ptr<Matrix<size_t>> coverageMatrix, bool multithread, bool* pathFound);
      //inline void AddItem();
   //signals:
      //void TimeUpdate(ProfilingData);
      //void TaskDataUpdate(QVariant);
      //void SubTaskDataUpdate(QVariant);
   private:
      //std::shared_ptr<Matrix<SVCG::route_point>> m_rawdata;
      //size_t m_rowCount, m_colCount;
      //std::shared_ptr<iMatrix> m_rawdata;
      //std::vector<std::future<route>> m_funAirVector;
      //std::vector<std::future<route>> m_funLandVector;
      //std::vector<std::future<route>> m_synchronizer;
      std::vector<TaskHolder> m_holders;
      std::vector<task_unit> m_taskPool;

      std::shared_ptr<settings::application_settings> m_appSettings;
      //XFM::Wrapper<LightPointData> m_data;
      //std::shared_ptr<Matrix<light_point_data>> m_lpData;
      //XFM::Wrapper<size_t> m_pointScore;

      // NOTE: Опасно!!!
      path_finder_statistic* m_statistic;

      std::unique_ptr<CoverageBuilder> m_coverageBuilder;
      std::unique_ptr<StrategyManager> m_strategyManager;

      ExperimentMeta m_vmeta;
      size_t m_rowCount;
      size_t m_colCount;
   };
}