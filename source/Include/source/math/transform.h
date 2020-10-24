#pragma once
#include "colreg/CommonStructs.h"
#include "SVCG/polar_point.h"
#include "SVCG/geo_point.h"
#include <cmath>

namespace SV::math
{
   /**
   * \class Transform
   * Для преоброзование из одной координатной системы в другую
   */
   template <class TransforImpl, class LocalPoint, class GlobalPoint>
   class Transform : protected TransforImpl
   {
   public:
      Transform(const CG::geo_point& center, double course)
         : _center(center)
         , _course(course)
      {}
      Transform(){}

      void SetCenter(const CG::geo_point& center, double course)
      {
         _center = center;
         _course = course;
      }

      LocalPoint ToLocal(const GlobalPoint& p) const { return toLocal(p, _center, _course); }
      GlobalPoint ToGlobal(const LocalPoint& p) const{ return toGlobal(p, _center, _course); }

      const GlobalPoint& GetCenter() const { return _center; }
      double GetCourse() const { return _course; }

   private:
      GlobalPoint _center;
      double    _course;
   };


   /**
   * \struct GeoToPolar
   * Реализация преоброзования из координатной системы проекции меркатора в полярную систему координат с заданным центром и поворотом
   */
   struct GeoToPolar
   {
      inline CG::polar_point toLocal(const CG::geo_point& p, const CG::geo_point& center, double course) const{ return { direction(center, p) - course, distance(center, p) }; }
      inline CG::geo_point toGlobal(const CG::polar_point& p, const CG::geo_point& center, double course) const{ return calc_point(center, p.distance, p.course + course); }
   };

   /**
   * \struct GeoToPolar
   * Реализация преоброзования из координатной системы проекции меркатора в полярную систему координат с заданным центром и поворотом с осями в метрах
   */
   struct GeoToPolarMetric
   {
      inline CG::polar_point toLocal(const CG::geo_point& p, const CG::geo_point& center, double course) const
      { 
         double dir, dist;
         distance_direction(center, p, dist, dir);
         return{ math::normal_K180(dir - course), dist * MILE }; 
      }
      inline CG::geo_point toGlobal(const CG::polar_point& p, const CG::geo_point& center, double course) const { return calc_point(center, p.distance * METERS_TO_MILE, p.course + course); }
   };


   /**
   * \struct GeoToСartesian
   * Реализация преоброзования из координатной системы проекции меркатора в декартову систему координат с осями в милях
   */
   struct GeoToCartesian
   {
      //GeoToCartesian
      inline point toLocal(const CG::geo_point& p, const CG::geo_point& center, double angle)const
      {
         const double course = math::grad_to_rad(math::direction(center, p) + angle);
         const double dist = math::distance(center, p);
         return point{ std::cos(course) * dist, std::sin(course) * dist };
      }

      //CartesianToGeo
      inline CG::geo_point toGlobal(const point& canonic, const CG::geo_point& center, double angle)const
      {
         return calc_point(center, std::hypot(canonic.x, canonic.y), rad_to_grad(std::atan2(canonic.x, canonic.y)) - angle);
      }
   };

   using GeoToPolarTransform = Transform<GeoToPolar, CG::polar_point, CG::geo_point>;
   using GeoToPolarMetricTransform = Transform<GeoToPolarMetric, CG::polar_point, CG::geo_point>;
   using GeoToCartasianTransform = Transform<GeoToCartesian, math::point, CG::geo_point>;
}
