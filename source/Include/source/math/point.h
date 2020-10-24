#pragma once

#include "math.h"

namespace SV::math
{
   struct point
   {
      point()
         :y{ 0. }
         ,x{ 0. }
      {}

      point( double y_, double x_)
         :y{ y_ }
         ,x{ x_ }
      {}
      union
      {
         double y;
         double lat;
      };
      union
      {
         double x;
         double lon;
      };
   };

   inline point operator+ (const point& lhs, const point& rhs)
   {
      return{ lhs.y + rhs.y, lhs.x + rhs.x };
   }

   inline point operator- (const point& lhs, const point& rhs)
   {
      return{ lhs.y - rhs.y, lhs.x - rhs.x };
   }

   inline point operator* (const point& pt, double k)
   {
      return{ pt.y * k, pt.x * k };
   }

   inline point operator* (double k, const point& pt)
   {
      return{ pt.y * k, pt.x * k };
   }

   inline bool operator== (const point& lhs, const point& rhs)
   {
      const auto eq = [](const auto& lhs, const auto& rhs) { return abs(lhs - rhs) < 1e-6; };
      return eq(lhs.x, rhs.x) && eq(lhs.y, rhs.y);
   }

   inline bool operator!= (const point& lhs, const point& rhs)
   {
      return !(lhs == rhs);
   }

   /*inline point& operator+= (const point& rhs)
   {
      x += rhs.x, y += rhs.y;
      return *this;
   }

   inline point& operator-= (const point& rhs)
   {
      x -= rhs.x, y -= rhs.y;
      return *this;
   }

   inline point operator- () const { return{ -x, -y }; }*/
}
