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
#include "Multithread/TaskHolder.h"

namespace SV::pathfinder
{
   class PathFinder
   {
      //INST_OBJECT(PathFinder)
   public:
      PathFinder();
   public:
      ~PathFinder();
   public:
      void FindLandPath(settings::route& route, const std::shared_ptr<RoutePointMatrix>& rawdata, const std::shared_ptr<Matrix<size_t>> coverageMatrix, bool multithread, bool* pathfound);
      void FindAirPath(settings::route& route, const std::shared_ptr<RoutePointMatrix>& rawdata, size_t iterations, bool multithread);
   private:
      CG::route_line findUniversalPath(CG::route_point& start, CG::route_point& finish, path_finder_logic& logic, const std::shared_ptr<RoutePointMatrix> rawdata, std::shared_ptr<Matrix<size_t>> coverageMatrix, bool multithread, bool* pathFound);
   };
}