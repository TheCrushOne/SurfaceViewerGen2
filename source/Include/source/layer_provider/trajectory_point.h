#pragma once

#include "common/base_types.h"
#include "SVCG/geo_point.h"

namespace SV::CG::layer_provider
{
   struct trajectory_point
   {
      trajectory_point()
         : speed{ NO_VALUE }
         , radius{ NO_VALUE }
         , left_XTE{ NO_VALUE }
         , right_XTE{ NO_VALUE }
      {}

      trajectory_point(const geo_point& p, double v_ = -1, double r = NO_VALUE, double lEXT = NO_VALUE, double rEXT = NO_VALUE)
         : pos(p)
         , speed(v_)
         , radius(r)
         , left_XTE{ lEXT }
         , right_XTE{ rEXT }
      {}

      trajectory_point(const trajectory_point&) = default;
      trajectory_point(trajectory_point&&)noexcept = default;
      trajectory_point& operator= (const trajectory_point& other) = default;
      trajectory_point& operator= (trajectory_point&& other)noexcept = default;

      inline bool operator == (const trajectory_point& b) const { return pos == b.pos; }
      inline operator geo_point() const { return pos; }
      inline bool is_valid() const { return pos.is_valid(); }

      geo_point pos;
      union
      {
         double    heading;   ///< Направление, градусы
         double    course;    ///< Курс, градусы
      };
      double    speed;     ///< Скорость, узлы
      double    radius;    ///< Радиус поворота, мили 
      double    left_XTE;  ///< Максимальное боковое смещение слева от линии маршрута, мили
      double    right_XTE; ///< Максимальное боковое смещение справа от линии маршрута, мили
      double    time;      ///< Время точки трека
   };

   typedef std::vector<trajectory_point> trajectory_point_vct;
}