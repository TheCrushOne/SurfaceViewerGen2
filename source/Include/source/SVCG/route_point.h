#pragma once

#include "geo_point.h"
//#include "pixel_point.h"
//#include "math_base_types.h"
#include "common/surface_base_types.h"

#include <cmath>

namespace SVCG
{
   struct route_point : public geo_point
   {
      surface_viewer::FlyZoneAffilation fly;
      surface_viewer::GoZoneAffilation go;
      bool is_control;
      route_point(double lat, double lon, double alt, surface_viewer::FlyZoneAffilation fly, surface_viewer::GoZoneAffilation go, bool isControl = false)
         : SVCG::geo_point{ lat, lon, alt }
         , fly(fly)
         , go(go)
         , is_control(isControl)
      {}
      route_point(double lat, double lon, double alt, bool isControl = false)
         : SVCG::geo_point{ lat, lon, alt }
         , fly(surface_viewer::FlyZoneAffilation::FZA_NORMAL)
         , go(surface_viewer::GoZoneAffilation::GZA_NORMAL)
         , is_control(isControl)
      {}
      route_point(bool isControl = false)
         : SVCG::geo_point{ 0., 0., 0. }
         , fly(surface_viewer::FlyZoneAffilation::FZA_NORMAL)
         , go(surface_viewer::GoZoneAffilation::GZA_NORMAL)
         , is_control(isControl)
      {}

      //const geo_point& operator()() { return *this; }
      //const pixel_point ToPixelPoint() { return pixel_point{ std::round(lat), std::round(lon),  std::round(alt) }; }

      //int x() { return ToPixelPoint().x; }
      //int y() { return ToPixelPoint().y; }
   };
}