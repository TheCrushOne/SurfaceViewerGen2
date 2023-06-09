#pragma once

#include <vector>
#include "colreg/CommonStructs.h"

namespace SVCG
{
   struct position_point : colreg::geo_point
   {
      position_point(double lt, double ln)
         : colreg::geo_point(lt, ln)
      {}
      position_point(double lt, double ln, double at)
         : colreg::geo_point(lt, ln)
         , alt(at)
      {}
      position_point() = default;
      position_point(const position_point&) = default;
      position_point(position_point&&)noexcept = default;
      position_point& operator= (const position_point& other) = default;
      position_point& operator= (position_point&& other)noexcept = default;

      bool operator==(const position_point& other) const { return lon == other.lon && lat == other.lat && alt == other.alt; }
      bool operator!=(const position_point& other) const { return !(*this == other); }

      double alt = colreg::NO_VALUE;
   };

   using position_points = std::vector<position_point>;
}