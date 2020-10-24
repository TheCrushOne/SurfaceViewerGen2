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
      void FindLandPath(settings::route& route, const SharedRoutePointMatrix& rawdata, const SharedUnsignedMatrix& coverageMatrix, bool multithread, bool* pathfound);
      void FindAirPath(settings::route& route, const SharedRoutePointMatrix& rawdata, size_t iterations, bool multithread);
   private:
      CG::route_line findUniversalPath(const CG::route_point& start, const CG::route_point& finish, const path_finder_logic& logic, const SharedRoutePointMatrix& rawdata, const SharedUnsignedMatrix& coverageMatrix, bool multithread, bool* pathFound);
   };
}