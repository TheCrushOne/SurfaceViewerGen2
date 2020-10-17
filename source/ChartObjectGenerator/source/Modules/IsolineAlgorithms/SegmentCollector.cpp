#include "stdafx.h"
#include "SegmentCollector.h"
#include "SVCG/positioning.h"

using namespace chart_object;
std::recursive_mutex g_segmentCollectorMutex;

// NOTE: минимальное покрытие скипает очень много точек...
chart_object::chart_object_unit_vct SegmentCollector::generateIsolineLevel(const pathfinder::GeoMatrix& rawdata, double height, int H)
{
   ATLASSERT(false);
   settings::environment_settings env_stt;
   const auto& envstt = /*GetPack()->settings->*/env_stt;
   SVCG::geo_contour isoPoints;
   constexpr double eps = 0.0001;

   for (size_t rIdx = 0; rIdx < rawdata.GetRowCount() - 1; rIdx++)
   {
      //auto& rowHigh = rawdata.arr[rIdx + 1];
      //auto& rowLow = rawdata.arr[rIdx];
      for (size_t cIdx = 0; cIdx < rawdata.GetColCount() - 1; cIdx++)
      {
         // NOTE: нули расположены снизу слева
         int iCIdx = static_cast<int>(cIdx);
         int iRIdx = static_cast<int>(rIdx);
         SVCG::geo_point gplu = SVCG::RoutePointToPositionPoint(SVCG::route_point{ iRIdx + 1, iCIdx }, envstt);
         SVCG::geo_point gpll = SVCG::RoutePointToPositionPoint(SVCG::route_point{ iRIdx, iCIdx }, envstt);
         SVCG::geo_point gpru = SVCG::RoutePointToPositionPoint(SVCG::route_point{ iRIdx + 1, iCIdx + 1 }, envstt);
         SVCG::geo_point gprl = SVCG::RoutePointToPositionPoint(SVCG::route_point{ iRIdx, iCIdx + 1 }, envstt);
         double hlu = rawdata.Get(rIdx + 1, cIdx);
         double hll = rawdata.Get(rIdx, cIdx);
         double hru = rawdata.Get(rIdx + 1, cIdx + 1);
         double hrl = rawdata.Get(rIdx, cIdx + 1);
         auto checkPoint = [eps, &isoPoints](SVCG::geo_point pt)->bool
         {
            return (std::find_if(isoPoints.begin(), isoPoints.end(), [pt, eps](const SVCG::geo_point& rs) { return math::isEqual(rs, pt, eps); }) == isoPoints.end());
         };
         if ((height - hll) * (height - hrl) <= 0)
         {
            if (hll == hrl)
            {
               if (checkPoint(gpll))
                  isoPoints.emplace_back(gpll);
               if (checkPoint(gprl))
                  isoPoints.emplace_back(gprl);
            }
            else
            {
               SVCG::geo_point gp = gpll;
               double heightPerLat = (hrl - hll) / (gprl.lat - gpll.lat);
               gp.lat = (height - hll) / heightPerLat + gpll.lat;
               if (checkPoint(gp))
                  isoPoints.emplace_back(gp);
            }
         }
         if ((height - hll) * (height - hlu) <= 0)
         {
            if (hll == hlu)
            {
               if (checkPoint(gpll))
                  isoPoints.emplace_back(gpll);
               if (checkPoint(gplu))
                  isoPoints.emplace_back(gplu);
            }
            else
            {
               SVCG::geo_point gp = gplu;
               double heightPerLon = (hlu - hll) / (gplu.lon - gpll.lon);
               gp.lon = (height - hll) / heightPerLon + gpll.lon;
               if (checkPoint(gp))
                  isoPoints.emplace_back(gp);
            }
         }
         if ((height - hrl) * (height - hru) <= 0)
         {
            if (hrl == hru)
            {
               if (checkPoint(gprl))
                  isoPoints.emplace_back(gprl);
               if (checkPoint(gpru))
                  isoPoints.emplace_back(gpru);
            }
            else
            {
               SVCG::geo_point gp = gpru;
               double heightPerLon = (hru - hrl) / (gpru.lon - gprl.lon);
               gp.lon = (height - hrl) / heightPerLon + gprl.lon;
               if (checkPoint(gp))
                  isoPoints.emplace_back(gp);
            }
         }
         if ((height - hlu) * (height - hru) <= 0)
         {
            if (hlu == hru)
            {
               if (checkPoint(gplu))
                  isoPoints.emplace_back(gplu);
               if (checkPoint(gpru))
                  isoPoints.emplace_back(gpru);
            }
            else
            {
               SVCG::geo_point gp = gplu;
               double heightPerLat = (hru - hlu) / (gpru.lat - gplu.lat);
               gp.lat = (height - hlu) / heightPerLat + gplu.lat;
               if (checkPoint(gp))
                  isoPoints.emplace_back(gp);
            }
         }
      }
   }

   // NOTE: Распределение точек по изолиниям
   // WARNING: костыль!!!
   auto pt1 = SVCG::RoutePointToPositionPoint({ 0, 0 }, envstt);
   auto pt2 = SVCG::RoutePointToPositionPoint({ 1, 1 }, envstt);
   m_maxRadius = math::distance(pt2, pt1);
   SVCG::geo_contour_vct isoLineVct;
   //isoLineVct.emplace_back();
   //for (auto& elem : isoPoints)
   //   isoLineVct.back().emplace_back(elem);
   auto nearestPointSetComparator = [](const std::vector<SVCG::geo_point>& check, const SVCG::geo_contour& nearest, SVCG::geo_point isoPoint, double& nearestMinDist) -> bool
   {
      auto isocheckmin = std::min_element(check.begin(), check.end(),
         [&isoPoint](const SVCG::geo_point& ptCheck, const SVCG::geo_point& nearest) -> bool
         {
            return math::distance(ptCheck, isoPoint) < math::distance(nearest, isoPoint);
         }
      );
      double checkDist = (isocheckmin == check.end() ? 0. : math::distance(*isocheckmin, isoPoint));
      auto isosmallestmin = std::min_element(nearest.begin(), nearest.end(),
         [&isoPoint](const SVCG::geo_point& ptCheck, const SVCG::geo_point& nearest) -> bool
         {
            return math::distance(ptCheck, isoPoint) < math::distance(nearest, isoPoint);
         }
      );
      double smallestDist = (isosmallestmin == nearest.end() ? 0. : math::distance(*isosmallestmin, isoPoint));
      ATLASSERT(checkDist != 0. && smallestDist != 0.);
      nearestMinDist = (checkDist < smallestDist) ? checkDist : smallestDist;
      return checkDist < smallestDist;
   };
   for (size_t isoIdx = 0; isoIdx < isoPoints.size(); isoIdx++)
   {
      auto& isoPoint = isoPoints.at(isoIdx);
      double min = std::numeric_limits<double>::max();
      double nearestMinDist = std::numeric_limits<double>::max();
      auto nearestIsoLine = std::min_element(isoLineVct.begin(), isoLineVct.end(),
         [&isoPoint, &nearestMinDist, nearestPointSetComparator](const SVCG::geo_contour& check, const SVCG::geo_contour& nearest)->bool
         {
            return nearestPointSetComparator(check, nearest, isoPoint, nearestMinDist);
         }
      );
      if (nearestIsoLine == isoLineVct.end() || nearestMinDist > m_maxRadius)
      {
         isoLineVct.emplace_back();
         nearestIsoLine = isoLineVct.end() - 1;
      }
      // NOTE: ближайшая точка к текущему изопоинту на изолинии
      //auto isoLineIter = std::min_element(nearestIsoLine->begin(), nearestIsoLine->end(),
      //   [isoPoint](const colreg::geo_point& ptCheck, const colreg::geo_point& smallest) -> bool
      //   { 
      //      return math::distance(ptCheck, isoPoint) < math::distance(smallest, isoPoint);
      //   }
      //);
      //if (isoLineIter == nearestIsoLine->end())
      nearestIsoLine->emplace_back(isoPoint);
      //else
         //nearestIsoLine->insert(isoLineIter, isoPoint);
   }

   // NOTE: слияние множеств точек изолиний на близком расстоянии
   bool smthMerged = true;
   while (smthMerged)
   {
      smthMerged = false;
      for (size_t outerIdx = 0; outerIdx < isoLineVct.size(); outerIdx++)
      {
         for (size_t innerIdx = 0; innerIdx < isoLineVct.size(); innerIdx++)
         {
            if (innerIdx == outerIdx)
               continue;

            double minDist = std::numeric_limits<double>::max();
            for (size_t ptIdx = 0; ptIdx < isoLineVct.at(outerIdx).size(); ptIdx++)
            {
               auto& isoPoint = isoLineVct.at(outerIdx).at(ptIdx);
               auto isosmallestmin = std::min_element(isoLineVct.at(innerIdx).begin(), isoLineVct.at(innerIdx).end(),
                  [&isoPoint](const SVCG::geo_point& ptCheck, const SVCG::geo_point& nearest) -> bool
                  {
                     return math::distance(ptCheck, isoPoint) < math::distance(nearest, isoPoint);
                  }
               );
               ATLASSERT(isosmallestmin != isoLineVct.at(innerIdx).end());
               if (minDist > math::distance(*isosmallestmin, isoPoint))
                  minDist = math::distance(*isosmallestmin, isoPoint);
            }

            bool mergeable = (minDist <= m_maxRadius);
            if (mergeable)
            {
               isoLineVct.at(outerIdx).insert(isoLineVct.at(outerIdx).end(), isoLineVct.at(innerIdx).begin(), isoLineVct.at(innerIdx).end());
               isoLineVct.erase(isoLineVct.begin() + innerIdx);
               smthMerged = true;
               break;
            }
         }
         if (smthMerged)
            break;
      }
   }

   // NOTE: построение изолиний
   for (auto& pointList : isoLineVct)
   {
      // NOTE: создание набора отрезков из которых может быть создана линия
      //std::vector<std::pair<colreg::geo_point, colreg::geo_point>> vectorList;
      //for (auto& srcPoint : pointList)
      //{
      //   for (auto& dstPoint : pointList)
      //   {
      //      bool distOk = math::distance(srcPoint, dstPoint) <= m_maxRadius;
      //      auto res = std::find_if(vectorList.begin(), vectorList.end(),
      //         [srcPoint, dstPoint](const std::pair<colreg::geo_point, colreg::geo_point>& elem) -> bool
      //         {
      //            return ((srcPoint == elem.first && dstPoint == elem.second)
      //               || (srcPoint == elem.second && dstPoint == elem.first));
      //         }
      //      );
      //      if (distOk && srcPoint != dstPoint && res == vectorList.end())
      //         vectorList.emplace_back(std::pair<colreg::geo_point, colreg::geo_point>{ srcPoint, dstPoint });
      //      //else
      //         //GetPack()->comm->Message(ICommunicator::MessageType::MS_Error, "check failed %f %f -> %f %f [%i, %i, %i]", srcPoint.lat, srcPoint.lon, dstPoint.lat, dstPoint.lon, distOk, srcPoint != dstPoint, res == vectorList.end());
      //   }
      //}

      //// NOTE: сортировка по длине
      //std::sort(vectorList.begin(), vectorList.end(),
      //   [](std::pair<colreg::geo_point, colreg::geo_point>& check, std::pair<colreg::geo_point, colreg::geo_point>& smallest) -> bool
      //   {
      //      return math::distance(check.first, check.second) < math::distance(smallest.first, smallest.second);
      //   }
      //);

      auto cmp = [](SVCG::geo_point a, SVCG::geo_point b)->bool
      {
         return a.lon < b.lon || a.lon == b.lon && a.lat < b.lat;
      };

      auto cw = [](SVCG::geo_point a, SVCG::geo_point b, SVCG::geo_point c)->bool
      {
         return a.lon * (b.lat - c.lat) + b.lon * (c.lat - a.lat) + c.lon * (a.lat - b.lat) < 0;
      };

      auto ccw = [](SVCG::geo_point a, SVCG::geo_point b, SVCG::geo_point c)->bool
      {
         return a.lon * (b.lat - c.lat) + b.lon * (c.lat - a.lat) + c.lon * (a.lat - b.lat) > 0;
      };

      auto convex_hull = [cmp, cw, ccw](SVCG::geo_contour& a)
      {
         if (a.size() == 1)  return;
         std::sort(a.begin(), a.end(), cmp);
         SVCG::geo_point p1 = a[0], p2 = a.back();
         SVCG::geo_contour up, down;
         up.push_back(p1);
         down.push_back(p1);
         for (size_t i = 1; i < a.size(); ++i) {
            if (i == a.size() - 1 || cw(p1, a[i], p2)) {
               while (up.size() >= 2 && !cw(up[up.size() - 2], up[up.size() - 1], a[i]))
                  up.pop_back();
               up.push_back(a[i]);
            }
            if (i == a.size() - 1 || ccw(p1, a[i], p2)) {
               while (down.size() >= 2 && !ccw(down[down.size() - 2], down[down.size() - 1], a[i]))
                  down.pop_back();
               down.push_back(a[i]);
            }
         }
         a.clear();
         for (size_t i = 0; i < up.size(); ++i)
            a.push_back(up[i]);
         for (size_t i = down.size() - 2; i > 0; --i)
            a.push_back(down[i]);
      };

      convex_hull(pointList);

      //auto jointCounter = [vectorList](const colreg::geo_point& point) -> size_t
      //{
      //   size_t count = 0;
      //   for (auto& vector : vectorList)
      //   {
      //      if (vector.first == point || vector.second == point)
      //         count++;
      //   }
      //   return count;
      //};
      //auto startPoint = std::min_element(pointList.begin(), pointList.end(),
      //   [jointCounter](const colreg::geo_point& ptCheck, const colreg::geo_point& smallest) -> bool
      //   {
      //      return jointCounter(ptCheck) < jointCounter(smallest);
      //   }
      //);
      //colreg::route_point sp = *startPoint;
      //pointList.clear();
      //pointList.emplace_back(sp);
      //bool inRadiusExists = true;
      //while (inRadiusExists)
      //{
      //   auto& last = pointList.back();
      //   // NOTE: по идее, в сортированном массиве будет найдено минимальное ребро
      //   auto nearestJointVec = std::find_if(vectorList.begin(), vectorList.end(),
      //      [last](const std::pair<math::geo_point, math::geo_point>& check) -> bool
      //      {
      //         return last == check.first || last == check.second;
      //      }
      //   );
      //   if (nearestJointVec != vectorList.end())
      //   {
      //      pointList.emplace_back(last == nearestJointVec->first ? nearestJointVec->second : nearestJointVec->first);
      //      vectorList.erase(nearestJointVec);
      //   }
      //   else
      //      inRadiusExists = false;
      //}
      pointList.emplace_back(pointList.front());
   }

   // NOTE: слияние изолиний на близком расстоянии
   //bool smthMerged = true;
   //while (smthMerged)
   //{
   //   smthMerged = false;
   //   for (size_t outerIdx = 0; outerIdx < isoLineVct.size(); outerIdx++)
   //   {
   //      auto outerBorderPoint1 = isoLineVct.at(outerIdx).front();
   //      auto outerBorderPoint2 = isoLineVct.at(outerIdx).back();
   //      for (size_t innerIdx = 0; innerIdx < isoLineVct.size(); innerIdx++)
   //      {
   //         if (innerIdx == outerIdx)
   //            continue;
   //         auto innerBorderPoint1 = isoLineVct.at(innerIdx).front();
   //         auto innerBorderPoint2 = isoLineVct.at(innerIdx).back();

   //         bool mergeable11 = math::distance(outerBorderPoint1, innerBorderPoint1) < m_maxRadius;
   //         bool mergeable12 = math::distance(outerBorderPoint1, innerBorderPoint2) < m_maxRadius;
   //         bool mergeable21 = math::distance(outerBorderPoint2, innerBorderPoint1) < m_maxRadius;
   //         bool mergeable22 = math::distance(outerBorderPoint2, innerBorderPoint2) < m_maxRadius;
   //         if (mergeable11)
   //         {
   //            std::reverse(isoLineVct.at(outerIdx).begin(), isoLineVct.at(outerIdx).end());
   //            isoLineVct.at(outerIdx).insert(isoLineVct.at(outerIdx).end(), isoLineVct.at(innerIdx).begin(), isoLineVct.at(innerIdx).end());
   //            if (mergeable22)
   //               isoLineVct.at(outerIdx).emplace_back(*isoLineVct.at(outerIdx).begin());
   //            isoLineVct.erase(isoLineVct.begin() + innerIdx);
   //            smthMerged = true;
   //            break;
   //         }
   //         if (mergeable12)
   //         {
   //            std::reverse(isoLineVct.at(innerIdx).begin(), isoLineVct.at(innerIdx).end());
   //            std::reverse(isoLineVct.at(outerIdx).begin(), isoLineVct.at(outerIdx).end());
   //            isoLineVct.at(outerIdx).insert(isoLineVct.at(outerIdx).end(), isoLineVct.at(innerIdx).begin(), isoLineVct.at(innerIdx).end());
   //            if (mergeable21)
   //               isoLineVct.at(outerIdx).emplace_back(*isoLineVct.at(outerIdx).begin());
   //            isoLineVct.erase(isoLineVct.begin() + innerIdx);
   //            smthMerged = true;
   //            break;
   //         }
   //         if (mergeable21)
   //         {
   //            isoLineVct.at(outerIdx).insert(isoLineVct.at(outerIdx).end(), isoLineVct.at(innerIdx).begin(), isoLineVct.at(innerIdx).end());
   //            isoLineVct.erase(isoLineVct.begin() + innerIdx);
   //            smthMerged = true;
   //            break;
   //         }
   //         if (mergeable22)
   //         {
   //            std::reverse(isoLineVct.at(innerIdx).begin(), isoLineVct.at(innerIdx).end());
   //            isoLineVct.at(outerIdx).insert(isoLineVct.at(outerIdx).end(), isoLineVct.at(innerIdx).begin(), isoLineVct.at(innerIdx).end());
   //            isoLineVct.erase(isoLineVct.begin() + innerIdx);
   //            smthMerged = true;
   //            break;
   //         }
   //      }
   //      if (smthMerged)
   //         break;
   //   }
   //}

   settings::application_settings& app_stt = GetService()->GetSettingsSerializerHolder()->GetSettings();
   chart_object::chart_object_unit_vct res;
   auto& gcBack = res.emplace_back();
   for (auto& line : isoLineVct)
   {
      auto& cBack = gcBack.geom_contour_vct.emplace_back();
      for (auto& point : line)
         cBack.emplace_back(static_cast<SVCG::geo_point>(point));
   }

   std::lock_guard<std::recursive_mutex> guard(g_segmentCollectorMutex);
   return res;
}