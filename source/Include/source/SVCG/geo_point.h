#pragma once

#include <vector>
#include "colreg/CommonStructs.h"

namespace SVCG
{
   struct geo_point : colreg::geo_point
   {
      geo_point(double lt, double ln)
         : colreg::geo_point(lt, ln)
      {}
      geo_point(double lt, double ln, double at)
         : colreg::geo_point(lt, ln)
         , alt(at)
      {}
      geo_point() = default;
      geo_point(const geo_point&) = default;
      geo_point(geo_point&&)noexcept = default;
      geo_point& operator= (const geo_point & other) = default;
      geo_point& operator= (geo_point && other)noexcept = default;

      bool operator==(const geo_point & other) const { return lon == other.lon && lat == other.lat && alt == other.alt; }
      bool operator!=(const geo_point & other) const { return !(*this == other); }

      double alt = colreg::NO_VALUE;
   };

   using geo_points = std::vector<geo_point>;
}