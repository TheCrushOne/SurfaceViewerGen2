#pragma once

//#include "base/instance.h"
#include "main/engine.h"
#include <memory>
#include "algorithm/statistic.h"
#include <functional>
#include <iostream>
#include <algorithm>
#include "common/coordinates.h"
#include "SVCG/route_point.h"
#include "common/pathfinder_types.h"

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
      void FindPath(strategy_settings settings, std::shared_ptr<route_data>& route, const std::shared_ptr<Matrix<SVCG::route_point>>& rawdata, /*bool multithread = true, size_t countIdx = 0, size_t legnthIdx = 0, bool research = false, bool landPath = true, size_t packetSize = 0*/const path_finder_settings pathFinderSettings, path_finder_statistic& statistic);
   private:
      bool prepareControlPoint(strategy_settings settings, size_t iterations, std::vector<route>& landRoutes, std::vector<route>& airRoutes/*, const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& rawdata*/);
      //std::shared_ptr<SVM::iMatrix<size_t>> buildLandCoverage(StrategySettings settings, std::vector<Route>& airRoutes);
      bool landPathCoverage(strategy_settings settings, std::vector<route>& landRoutes, std::vector<route>& airRoutes);
      SVCG::route_point checkRetranslateFlyPointAffilation(int row, int col/*, const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& rawdata*/);
      route findLandPath(route& route/*, std::shared_ptr<SVM::iMatrix<size_t>>& coverageMatrix*/, bool multithread, bool* pathFounded);
      route findAirPath(route& route, size_t iterations, bool multithread);
      std::vector<SVCG::route_point> findPath(SVCG::route_point& start, SVCG::route_point& finish, path_finder_logic& logic/*, std::shared_ptr<SVM::iMatrix<size_t>>& coverageMatrix*/, bool multithread, bool* pathFound);
      void fillSupportData(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& rawdata*/);
      void clearSupportData();
      //inline void AddItem();
   //signals:
      //void TimeUpdate(ProfilingData);
      //void TaskDataUpdate(QVariant);
      //void SubTaskDataUpdate(QVariant);
   private:
      //std::shared_ptr<SVM::iMatrix<SurfaceElement>> m_rawdata;
      //size_t m_rowCount, m_colCount;
      //std::shared_ptr<iMatrix> m_rawdata;
      //std::vector<QFuture<Route>> m_funAirVector;
      //std::vector<QFuture<Route>> m_funLandVector;
      //QFutureSynchronizer<Route> m_synchronizer;

      //std::shared_ptr<settings::application_settings> m_appSettings;
      //XFM::Wrapper<LightPointData> m_data;
      //std::shared_ptr<SVM::iMatrix<SurfaceElement>> m_data;
      //XFM::Wrapper<size_t> m_pointScore;

      // NOTE: Опасно!!!
      path_finder_statistic* m_statistic;

      ExperimentMeta m_vmeta;
      size_t m_rowCount;
      size_t m_colCount;
   };
}