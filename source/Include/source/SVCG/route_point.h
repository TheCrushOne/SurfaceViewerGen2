#pragma once

//#include "geo_point.h"
//#include "pixel_point.h"
//#include "math_base_types.h"
//#include "common/surface_base_types.h"

#include <cmath>

namespace SVCG
{
   struct route_point
   {
      int row, col;
      double height;
      pathfinder::FlyZoneAffilation fly;
      pathfinder::GoZoneAffilation go;
      bool is_control;
      route_point(int row, int col, double height, pathfinder::FlyZoneAffilation fly, pathfinder::GoZoneAffilation go, bool isControl = false)
         : row(row)
         , col(col)
         , height(height)
         , fly(fly)
         , go(go)
         , is_control(isControl)
      {}
      route_point(int row, int col, double height, bool isControl = false)
         : row(row)
         , col(col)
         , height(height)
         , fly(pathfinder::FlyZoneAffilation::FZA_NORMAL)
         , go(pathfinder::GoZoneAffilation::GZA_NORMAL)
         , is_control(isControl)
      {}
      route_point(int row, int col, bool isControl = false)
         : row(row)
         , col(col)
         , height(0.)
         , fly(pathfinder::FlyZoneAffilation::FZA_NORMAL)
         , go(pathfinder::GoZoneAffilation::GZA_NORMAL)
         , is_control(isControl)
      {}
      route_point(bool isControl = false)
         : row(0)
         , col(0)
         , height(.0)
         , fly(pathfinder::FlyZoneAffilation::FZA_NORMAL)
         , go(pathfinder::GoZoneAffilation::GZA_NORMAL)
         , is_control(isControl)
      {}

      //const geo_point& operator()() { return *this; }
      //const pixel_point ToPixelPoint() { return pixel_point{ std::round(lat), std::round(lon),  std::round(alt) }; }

      //int x() { return ToPixelPoint().x; }
      //int y() { return ToPixelPoint().y; }
   };
}