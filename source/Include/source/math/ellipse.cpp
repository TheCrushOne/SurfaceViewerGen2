#include "StdAfx.h"
#include "ellipse.h"
#include "../Colreg/ThirdParty/poly34.h"
namespace math
{
   Ellipse::Ellipse(const colreg::geo_point& center, double majorRadius, double minorRadius, double course)
      : _center(center)
      , _majorRadius(majorRadius)
      , _minorRadius(minorRadius)
      , _angle(90 - course)
      , _course{ course }
   {}

   /*!
   Solving ellipse and line equations in parametric form.
   r(t) = (a * cos(t), b * sin(t)) //< Ellipse equation
   r(u) = u * (dx, dy) + v         //< Line equation
   \return vector of points situated both on line and ellipse
   */
   std::vector<colreg::geo_point> Ellipse::IntersectWithLine(const colreg::geo_point& from, const colreg::geo_point& to) const
   {
      const math::point ptFrom = toCanonic(from);
      const math::point ptTo = toCanonic(to);
      const math::point vDir = ptTo - ptFrom;
      const double dMajorSqr = _majorRadius * _majorRadius;
      const double dMinorSqr = _minorRadius * _minorRadius;
      const double dDySqr = vDir.y * vDir.y;
      const double dDxSqr = vDir.x * vDir.x;
      double roots[2];

      const size_t nRoots = math::quadratic(dDxSqr * dMinorSqr + dDySqr * dMajorSqr,
         2 * (ptFrom.x * vDir.x * dMinorSqr + ptFrom.y * vDir.y * dMajorSqr),
         ptFrom.x * ptFrom.x * dMinorSqr + ptFrom.y * ptFrom.y * dMajorSqr - dMinorSqr * dMajorSqr,
         roots);

      std::vector<colreg::geo_point> ptMercator;
      ptMercator.reserve(nRoots);

      for (size_t i = 0; i < nRoots; ++i)
         ptMercator.emplace_back(toMercator(roots[i] * vDir + ptFrom));

      return ptMercator;
   }


   /*!
   Solving ellipse and ellipse equations in parametric form.
   r(t) = (a * cos(t), b * sin(t)) //< This ellipse equation
   r(u) = (c * cos(u) * cos(w) - d * sin(u) * sin(w), c * cos(u) * sin(w) + d * sin(u) * cos(w)) + v //< Another ellipse general equation
   \return vector of points situated on both ellipses
   */
   std::vector<colreg::geo_point> Ellipse::IntersectWithEllipse(const Ellipse& ellipse) const
   {
      std::vector<colreg::geo_point> result;

      std::vector<double> tangents{ 0, 0, 0, 0 };

      const double a = _majorRadius;
      const double b = _minorRadius;
      const double c = ellipse._majorRadius;
      const double d = ellipse._minorRadius;
      const double angle = math::grad_to_rad(ellipse._angle - _angle);
      const double sinA = std::sin(angle);
      const double cosA = std::cos(angle);

      const math::point pt = toCanonic(ellipse._center);

      const double aSqr = a * a;
      const double bSqr = b * b;
      const double sinSqr = sinA * sinA;
      const double cosSqr = cosA * cosA;

      const double m1 = c * c * (aSqr * sinSqr + bSqr * cosSqr);
      const double m2 = c * (aSqr * pt.y * sinA + bSqr * pt.x * cosA);
      const double m3 = c * d * sinA * cosA * (aSqr - bSqr);
      const double m4 = d * (aSqr * pt.y * cosA - bSqr * pt.x * sinA);
      const double m5 = d * d * (aSqr * cosSqr + bSqr * sinSqr);
      const double m6 = aSqr * pt.y * pt.y + bSqr * pt.x * pt.x - aSqr * bSqr;

      const double k1 = m1 + m6 - 2 * m2;
      const double k2 = 4 * (m4 - m3);
      const double k3 = 2 * (2 * m5 + m6 - m1);
      const double k4 = 4 * (m3 + m4);
      const double k5 = m1 + 2 * m2 + m6;

      const double ratio = 1. / k1;

      tangents.resize(SolveP4(&tangents[0], k2 * ratio, k3 * ratio, k4 * ratio, k5 * ratio));

      for (double tangent : tangents)
      {
         const double dTanSqr = tangent * tangent;
         const double dRatio = 1. / (1. + dTanSqr);
         const double dSin = 2 * tangent * dRatio;
         const double dCos = (1. - dTanSqr) * dRatio;

         result.emplace_back(toMercator(math::point{c * dCos * sinA + d * dSin * cosA,  c * dCos * cosA - d * dSin * sinA } +pt));
      }

      return result;
   }

   math::point Ellipse::toCanonic(const colreg::geo_point& pt) const
   {
      const double course = math::grad_to_rad(math::direction(_center, pt) + _angle);
      const double dist = math::distance(_center, pt);

      return math::point{ std::cos(course) * dist, std::sin(course) * dist };
   }

   colreg::geo_point Ellipse::toMercator(const math::point& canonic) const
   {
      return math::calc_point(_center, std::hypot(canonic.x, canonic.y), math::rad_to_grad(std::atan2(canonic.x, canonic.y)) - _angle);
   }

}