/*!
\file
\brief Заголовочный файл ellipse.h
Математика с эллипсом
*/

#pragma once
#include "point.h"
#include "colreg/CommonStructs.h"
#include "math_utils.h"

namespace math
{
   class Ellipse
   {
   public:
      Ellipse() = default;
      /*!
      \param[in] center Ellipse center geographic coordinates
      \param[in] majorRadius Ellipse major axis radius (directed towards ellipse course) in miles
      \param[in] minorRadius Ellipse minor axis radius in miles
      \param[in] course Ellipse course in degrees as used in Mercator Projection
      */
      Ellipse(const colreg::geo_point& center, double majorRadius, double minorRadius, double course);

      /*!
      *Получить точки пересечения эллипса и линии, проходящей через точки from-to, в меркаторе
      */
      std::vector<colreg::geo_point> IntersectWithLine(const colreg::geo_point& from, const colreg::geo_point& to) const;

      /*!
      *Получить точки пересечения эллипса с другим эллипсом, в меркаторе
      */
      std::vector<colreg::geo_point> IntersectWithEllipse(const Ellipse& ellipse) const;

      /*!
      *Получить центр эллипса в меркаторе
      */
      inline const auto& GetCenter()const { return _center; }

      /*!
      *Получить большой радиус эллипса, в милях
      */
      inline auto GetMajorRadius()const { return _majorRadius; }

      /*!
      *Получить малый радиус эллипса, в милях
      */
      inline auto GetMinorRadius()const { return _minorRadius; }

      /*!
      *Получить центр эллипса в меркаторе
      */
      inline auto GetMajorPoint()const { return math::calc_point(_center, _majorRadius, _course); }

      /*!
      *Получить центр эллипса в меркаторе
      */
      inline auto GetMinorPoint()const { return math::calc_point(_center, _minorRadius, _course - 90); }

      /*!
      *Получить точку в меркаторе на дуге эллипса по углу относительно оси большого радиуса
      */
      inline auto GetArcPoint(double angle)const { return math::calc_point(_center, GetRadius(_course + angle), _course + angle); }

      /*!
      *Получить радиус эллипса по углу в меркаторе, мили
      */
      inline double GetRadius(double angle)const
      {
         angle -= _course;
         if(math::isEqual(fabs(angle), 0.,  0.1))
            return _majorRadius;
         if (math::isEqual(fabs(angle), 90., 0.1) || math::isEqual(fabs(angle), 270., 0.1))
            return _minorRadius;
         return _majorRadius * _minorRadius / std::hypot(_minorRadius * math::cos_grad(angle), _majorRadius * math::sin_grad(angle));
      }

      /*!
      Вернуть угол эллипса к точке, образованной пересечением примой проходящей на расстоянии splitDist в милях от центра эллипса и перпендикулярой его главной оси(курсу)
      */
      inline double SplitAngle(double splitDist)const
      {
         return math::rad_to_grad(atan(sqrt((1 - splitDist*splitDist / (_majorRadius*_majorRadius))*_minorRadius*_minorRadius) / splitDist));
      }

   private:
      /*!
      Returns coordinate so that the x-axis lies along the major axis
      \param[in] pt
      */
      math::point toCanonic(const colreg::geo_point& pt) const;

      /*!
      Reverse transform from ellipse canonic system to Mercator projection
      */
      colreg::geo_point toMercator(const math::point& canonic) const;

      colreg::geo_point _center; //< Geographic center of the ellipse
      double _majorRadius;     //< Ellipse major radius
      double _minorRadius;     //< Ellipse minor radius
      double _angle;           //< Ellipse math angle (starting at 0 math deg and directed CCW)
      double _course;          //< Ellipse course in degrees as used in Mercator Projection
   };

   using Ellipses = std::vector<math::Ellipse>;
}