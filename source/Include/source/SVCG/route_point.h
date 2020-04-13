#pragma once

//#include "geo_point.h"
//#include "pixel_point.h"
//#include "math_base_types.h"
#include "common/pathfinder_types.h"
#include "colreg/CommonStructs.h"

#include <cmath>

namespace SVCG
{
   struct route_point
   {
      int row = colreg::INVALID_INT_ID, col = colreg::INVALID_INT_ID;
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

      bool operator==(const route_point& elem)
      {
         return this->row == elem.row
            && this->col == elem.col
            && this->go == elem.go
            && this->fly == elem.fly
            && this->height == elem.height
            && this->is_control == elem.is_control;
      }
      //const geo_point& operator()() { return *this; }
      //const pixel_point ToPixelPoint() { return pixel_point{ std::round(lat), std::round(lon),  std::round(alt) }; }

      //int x() { return ToPixelPoint().x; }
      //int y() { return ToPixelPoint().y; }
   };
}