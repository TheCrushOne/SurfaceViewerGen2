#pragma once

#include "route_point.h"
#include "position_point.h"
#include "common/settings.h"
#include "math/point.h"

namespace SVCG
{
   inline route_point PositionPointToRoutePoint(const position_point& pPoint, const settings::environment_settings settings = settings::environment_settings())
   {
      
      route_point rPoint(pPoint.lon* ,);
      pPoint.lat;
      pPoint.lon;
      pPoint.alt;
      return rPoint;
   }

   inline math::point PositionPointToPixelPoint(const position_point& pPoint, const settings::environment_settings settings = settings::environment_settings())
   {

   }
}