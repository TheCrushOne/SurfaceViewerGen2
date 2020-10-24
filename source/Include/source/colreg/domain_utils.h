#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "CommonStructs.h"
#include "DomainInterface.h"

namespace colreg
{

namespace math
{
   using std::sin;
   using std::cos;
   using std::floor;
   using std::atan2;
   using std::hypot;
   using std::abs;

//   inline point to_cart(const polar& p)
//   {
//      const double rad = deg_to_rad(p.course);
//      return point{ p.distance * sin(rad), p.distance * cos(rad) };
//   }
//
//   inline polar to_polar(const point& pt)
//   {
//      return{ normalize(rad_to_deg(atan2(pt.x, pt.y))), hypot(pt.x, pt.y) };
//   }
//
//   inline point rotate_coordinate(const point& pt, const double angle)
//   {
//      return{ pt.x*cos(deg_to_rad(angle)) - pt.y*sin(deg_to_rad(angle)), pt.x*sin(deg_to_rad(angle)) + pt.y*cos(deg_to_rad(angle)) };
//   }
//
//   inline std::vector<polar> ellipse_contour(const polar* geom, double precision)
//   {
//      constexpr size_t centerIdx = size_t(DOGT_ARC_PART::DOGT_ARC_CENTER);
//      constexpr size_t majorIdx = size_t(DOGT_ARC_PART::DOGT_ARC_MAJOR);
//      constexpr size_t minorIdx = size_t(DOGT_ARC_PART::DOGT_ARC_MINOR);
//      constexpr size_t beginIdx = size_t(DOGT_ARC_PART::DOGT_ARC_BEGIN);
//      constexpr size_t endIdx = size_t(DOGT_ARC_PART::DOGT_ARC_END);
//
//      const point centerPt = to_cart(geom[centerIdx]);
//      const point majorPt = to_cart(geom[majorIdx]);
//      const point minorPt = to_cart(geom[minorIdx]);
//      const point beginPt = to_cart(geom[beginIdx]);
//      const point endPt = to_cart(geom[endIdx]);
//
//      const double transformCourse = direction(centerPt, majorPt);
//      const double startAngle = normal_K360(direction(centerPt, beginPt) - transformCourse);
//      double endAngle = normal_K360(direction(centerPt, endPt) - transformCourse);
//      
//      if (endAngle < startAngle)
//         endAngle += 360;
//      double angleDelta = endAngle - startAngle;
//
//      const double majorRadius = distance(centerPt, majorPt);
//      const double minorRadius = distance(centerPt, minorPt);
//
//      std::vector<polar> result;
//
//      point pnt = beginPt - centerPt;
//      pnt = rotate_coordinate(pnt, transformCourse);
//      
//      double step = precision / meters_in_mile;
//           
//      result.emplace_back(to_polar(beginPt));
//      
//      double angleSumm = 0, curAngle = 0, kt = 0, nwAngle = 0;
//      point pntWOCorrect, nwPoint;
//      curAngle = direction({ 0, 0 }, pnt);
//
//      while (angleDelta > angleSumm)
//      {
//         pnt.y += -(step*(pnt.x) / minorRadius);
//         pnt.x += (step*(pnt.y) / majorRadius);
//                  
//         pntWOCorrect = pnt;
//                           
//         kt = pnt.y / pnt.x;
//         pnt.x = minorRadius * majorRadius / sqrt(pow(majorRadius, 2.) + pow(minorRadius*kt, 2.));
//         if (pntWOCorrect.x*pnt.x < 0)
//            pnt.x *= -1.;
//         pnt.y = kt*pnt.x;
//
//         nwPoint = rotate_coordinate(pnt, -transformCourse);
//
//         result.emplace_back(to_polar(nwPoint + centerPt));
//
//         nwAngle = direction({ 0, 0 }, pnt);
//         if (nwAngle < curAngle)
//            nwAngle += 360;
//         angleSumm += abs(curAngle - nwAngle);
//
//         curAngle = nwAngle;
//      }
//      
//      result.pop_back();
//      result.emplace_back(to_polar(endPt));
//
//      return result;
//   }
}
//
//   /*!
//   Get domain topology as a polyline with maximum curves interpolating distance of @precision, meters
//   /param[in] topology safety domain topology
//   /param[in] precision maximum distance for curves interpolation points, meters
//   */
//   inline std::vector<polar> calculate_domain_contour(const domain_geometry_ref* topology, double precision)
//   {
//      std::vector<polar> result;
//      result.reserve(32);
//      const auto insert_topology = [&](const std::vector<polar>& points) { result.insert(result.end(), points.begin(), points.end()); };
//
//      for (size_t i = 0; i < topology->size; ++i)
//      {
//         const domain_geometry& geom = topology->arr[i];
//
//         switch (geom.type)
//         {
//         case DOMAIN_GEOMETRY_TYPE::DOGT_ARC:    insert_topology(math::ellipse_contour(geom.points.arr, precision)); break;
//         default:
//            result.insert(result.end(), geom.points.arr, geom.points.arr + geom.points.size);
//         }
//      }
//      if(!result.empty())result.push_back(result[0]);
//
//      return result;
//   }

}
