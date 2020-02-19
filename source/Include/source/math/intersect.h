#pragma once

namespace math
{

   inline bool intersect(double a1_y, double a1_x, double a2_y, double a2_x, double b1_y, double b1_x,
      double b2_y, double b2_x, double *c_y, double *c_x)
   {
      double dir1_y = a2_y - a1_y;
      double dir1_x = a2_x - a1_x;
      double dir2_y = b2_y - b1_y;
      double dir2_x = b2_x - b1_x;

      //считаем уравнения прямых проходящих через отрезки
      double a1 = -dir1_y;
      double b1 = +dir1_x;
      double d1 = -(a1*a1_x + b1*a1_y);

      double a2 = -dir2_y;
      double b2 = +dir2_x;
      double d2 = -(a2*b1_x + b2*b1_y);

      //подставляем концы отрезков, для выяснения в каких полуплоскотях они
      double seg1_line2_start = a2*a1_x + b2*a1_y + d2;
      double seg1_line2_end = a2*a2_x + b2*a2_y + d2;

      double seg2_line1_start = a1*b1_x + b1*b1_y + d1;
      double seg2_line1_end = a1*b2_x + b1*b2_y + d1;

      //если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
      if(seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0 )
         return false;

      double u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
      *c_y = a1_y + u*dir1_y;
      *c_x = a1_x + u*dir1_x;

      return true;
   }

}