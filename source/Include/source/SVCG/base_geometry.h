#pragma once

#include "common/base_types.h"

namespace SV::CG
{
   struct geo_point
   {
      geo_point(double lt, double ln)
         : lon(ln)
         , lat(lt)
      {}
      geo_point() = default;
      geo_point(const geo_point&) = default;
      geo_point(geo_point&&) noexcept = default;
      geo_point& operator= (const geo_point& other) = default;
      geo_point& operator= (geo_point&& other) noexcept = default;

      bool operator==(const geo_point& other) const { return lon == other.lon && lat == other.lat; }
      bool operator!=(const geo_point& other) const { return !(*this == other); }
      bool is_valid() const { return lat != NO_VALUE && lon != NO_VALUE; }

      double lon = NO_VALUE;
      double lat = NO_VALUE; ///< Координаты в градусах
   };

   typedef std::vector<geo_point> geo_contour;
   typedef std::vector<geo_contour> geo_contour_vct;
}