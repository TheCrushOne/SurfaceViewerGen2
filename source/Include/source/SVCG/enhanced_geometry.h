#pragma once

#include <vector>

namespace SV::CG
{
   struct point_2
   {
      point_2(double x, double y) {}
   };

   typedef std::vector<point_2> vector_2;

   struct PolyPolygon_2
   {
      bool IsPointInPolygon(const point_2& pt, bool* isOnEdge) { return false; }
      void AddContour(const vector_2& vct) {}
   };
}