#pragma once

#include "base_geometry.h"

namespace SVCG
{
   struct point_2
   {
      point_2(double x, double y) {}
   };

   using vector_2 = std::vector<point_2>;

   struct PolyPolygon_2
   {
      bool IsPointInPolygon(const point_2& pt, bool* isOnEdge) { return false; }
      void AddContour(const vector_2& vct) {}
   };
}