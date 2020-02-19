#pragma once
#include "colreg/CommonStructs.h"

namespace math
{
   /**
   * \class Transform
   * Для преоброзование из одной координатной системы в другую
   */
   template <class TransforImpl, class LocalPoint, class GlobalPoint>
   class Transform : private TransforImpl
   {
   public:
      Transform(const geo_point& center, double course)
         : _center(center)
         , _course(course)
      {}
      Transform(){}

      void SetCenter(const geo_point& center, double course)
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
      inline colreg::polar toLocal(const geo_point& p, const geo_point& center, double course) const{ return { direction(center, p) - course, distance(center, p) }; }
      inline geo_point toGlobal(const colreg::polar& p, const geo_point& center, double course) const{ return calc_point(center, p.distance, p.course + course); }
   };

   /**
   * \struct GeoToPolar
   * Реализация преоброзования из координатной системы проекции меркатора в полярную систему координат с заданным центром и поворотом с осями в метрах
   */
   struct GeoToPolarMetric
   {
      inline colreg::polar toLocal(const geo_point& p, const geo_point& center, double course) const 
      { 
         double dir, dist;
         distance_direction(center, p, dist, dir);
         return{ math::normal_K180(dir - course), dist * MILE }; 
      }
      inline geo_point toGlobal(const colreg::polar& p, const geo_point& center, double course) const { return calc_point(center, p.distance * METERS_TO_MILE, p.course + course); }
   };


   /**
   * \struct GeoToСartesian
   * Реализация преоброзования из координатной системы проекции меркатора в декартову систему координат с осями в милях
   */
   struct GeoToCartesian
   {
      //GeoToCartesian
      inline point toLocal(const colreg::geo_point& p, const colreg::geo_point& center, double angle)const
      {
         const double course = math::grad_to_rad(math::direction(center, p) + angle);
         const double dist = math::distance(center, p);
         return point{ std::cos(course) * dist, std::sin(course) * dist };
      }

      //CartesianToGeo
      inline geo_point toGlobal(const point& canonic, const colreg::geo_point& center, double angle)const
      {
         return calc_point(center, std::hypot(canonic.x, canonic.y), rad_to_grad(std::atan2(canonic.x, canonic.y)) - angle);
      }
   };

   using GeoToPolarTransform = Transform<GeoToPolar, colreg::polar, colreg::geo_point>; 
   using GeoToPolarMetricTransform = Transform<GeoToPolarMetric, colreg::polar, colreg::geo_point>;
   using GeoToCartasianTransform = Transform<GeoToCartesian, math::point, colreg::geo_point>;
}
