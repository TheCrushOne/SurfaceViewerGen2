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
      int x, y, z;
      pathfinder::FlyZoneAffilation fly;
      pathfinder::GoZoneAffilation go;
      bool is_control;
      route_point(int x, int y, int z, pathfinder::FlyZoneAffilation fly, pathfinder::GoZoneAffilation go, bool isControl = false)
         : x(x)
         , y(y)
         , z(z)
         , fly(fly)
         , go(go)
         , is_control(isControl)
      {}
      route_point(int x, int y, int z, bool isControl = false)
         : x(x)
         , y(y)
         , z(z)
         , fly(pathfinder::FlyZoneAffilation::FZA_NORMAL)
         , go(pathfinder::GoZoneAffilation::GZA_NORMAL)
         , is_control(isControl)
      {}
      route_point(bool isControl = false)
         : x(x)
         , y(y)
         , z(z)
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