#include "stdafx.h"
#include "IsolineGenerator.h"
#include "SVCG/positioning.h"
#include "common/pathfinder_structs.h"

using namespace chart_object;

void HSVtoRGB(int H, double S, double V, int output[3])
{
   double C = S * V;
   double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
   double m = V - C;
   double Rs, Gs, Bs;

   if (H >= 0 && H < 60) {
      Rs = C;
      Gs = X;
      Bs = 0;
   }
   else if (H >= 60 && H < 120) {
      Rs = X;
      Gs = C;
      Bs = 0;
   }
   else if (H >= 120 && H < 180) {
      Rs = 0;
      Gs = C;
      Bs = X;
   }
   else if (H >= 180 && H < 240) {
      Rs = 0;
      Gs = X;
      Bs = C;
   }
   else if (H >= 240 && H < 300) {
      Rs = X;
      Gs = 0;
      Bs = C;
   }
   else {
      Rs = C;
      Gs = 0;
      Bs = X;
   }

   output[0] = (Rs + m) * 255;
   output[1] = (Gs + m) * 255;
   output[2] = (Bs + m) * 255;
}

void IsolineGenerator::GenerateIsolines(const converter::raw_data_ref& rawdata)
{
   size_t levelCount = 10;
   double min = rawdata.arr[0].arr[0], max = rawdata.arr[0].arr[0];
   for (auto& row : rawdata)
   {
      for (auto& col : row)
      {
         if (col < min)
            min = col;
         if (col > max)
            max = col;
      }
   }
   double step = (max - min) / static_cast<double>(levelCount);
   //double height = 27.;
   //double height = 50.;

   // NOTE: менять тип алгоритма тут
   for (size_t levelIdx = 0; levelIdx < levelCount; levelIdx++)
      generateIsolineLevel(AlgorithmType::AT_RC1, rawdata, min + step * static_cast<double>(levelIdx), 360. / static_cast<double>(levelCount)* static_cast<double>(levelIdx));
}

void IsolineGenerator::generateIsolineLevel(AlgorithmType type, const converter::raw_data_ref& rawdata, double height, int H)
{
   switch(type)
   {
      case AlgorithmType::AT_RC1:
         generateRC1(rawdata, height, H);
         break;
      case AlgorithmType::AT_RC2:
         generateRC2(rawdata, height, H);
         break;
      default:
         ATLASSERT(false);
   }
}

void IsolineGenerator::generateRC1(const converter::raw_data_ref& rawdata, double height, int H)
{
   if (!rawdata.size)
      return;
   auto actValMtx = std::make_shared<pathfinder::Matrix<bool>>(rawdata.size, rawdata.arr[0].size, false);
   auto inLineFlagMtx = std::make_shared<pathfinder::Matrix<bool>>(rawdata.size, rawdata.arr[0].size, false);
   auto passedFlagMtx = std::make_shared<pathfinder::Matrix<bool>>(rawdata.size, rawdata.arr[0].size, false);

   for (size_t rIdx = 0; rIdx < rawdata.size; rIdx++)
   {
      auto& row = rawdata.arr[rIdx];
      for (size_t cIdx = 0; cIdx < row.size; cIdx++)
      {
         if (row[cIdx] >= height)
            actValMtx->Set(rIdx, cIdx, true);
      }
   }

   auto passedCheck = [&passedFlagMtx](int iRIdx, int iCIdx)->bool
   {
      if (iRIdx < 0
         || iCIdx < 0
         || iRIdx >= passedFlagMtx->GetRowCount()
         || iCIdx >= passedFlagMtx->GetColCount())
         return false;
      return passedFlagMtx->Get(static_cast<size_t>(iRIdx), static_cast<size_t>(iCIdx));
   };

   auto inLineCheck = [&inLineFlagMtx](int iRIdx, int iCIdx)->bool
   {
      if (iRIdx < 0
         || iCIdx < 0
         || iRIdx >= inLineFlagMtx->GetRowCount()
         || iCIdx >= inLineFlagMtx->GetColCount())
         return false;
      return inLineFlagMtx->Get(static_cast<size_t>(iRIdx), static_cast<size_t>(iCIdx));
   };

   auto activationCheck = [&actValMtx](int iRIdx, int iCIdx)->bool
   {
      if (iRIdx < 0
         || iCIdx < 0
         || iRIdx >= actValMtx->GetRowCount()
         || iCIdx >= actValMtx->GetColCount())
         return false;
      return actValMtx->Get(static_cast<size_t>(iRIdx), static_cast<size_t>(iCIdx));
   };

   auto nearestActivationCheck = [&actValMtx, activationCheck](size_t rIdx, size_t cIdx)->bool
   {
      size_t actCount = 0;
      int iRIdx = static_cast<int>(rIdx);
      int iCIdx = static_cast<int>(cIdx);
      actCount += activationCheck(iRIdx + 1, iCIdx - 1) ? 1 : 0;
      actCount += activationCheck(iRIdx + 1, iCIdx) ? 1 : 0;
      actCount += activationCheck(iRIdx + 1, iCIdx + 1) ? 1 : 0;
      actCount += activationCheck(iRIdx, iCIdx + 1) ? 1 : 0;
      actCount += activationCheck(iRIdx - 1, iCIdx + 1) ? 1 : 0;
      actCount += activationCheck(iRIdx - 1, iCIdx) ? 1 : 0;
      actCount += activationCheck(iRIdx - 1, iCIdx - 1) ? 1 : 0;
      actCount += activationCheck(iRIdx, iCIdx - 1) ? 1 : 0;
      return actCount < 7;
   };

   for (size_t rIdx = 0; rIdx < rawdata.size; rIdx++)
   {
      auto& row = rawdata.arr[rIdx];
      for (size_t cIdx = 0; cIdx < row.size; cIdx++)
         inLineFlagMtx->Set(rIdx, cIdx, activationCheck(rIdx, cIdx) && nearestActivationCheck(rIdx, cIdx));
   }

   auto pointCheck = [&activationCheck, &inLineCheck, &passedCheck, &inLineFlagMtx, &passedFlagMtx](SVCG::route_point& res, TraversalDirection dir, size_t rIdx, size_t cIdx)->bool
   {
      int toff = static_cast<int>(rIdx) + traversalOffset.at(dir).first.first;
      int tofs = static_cast<int>(cIdx) + traversalOffset.at(dir).first.second;
      int tosf = static_cast<int>(rIdx) + traversalOffset.at(dir).second.first;
      int toss = static_cast<int>(cIdx) + traversalOffset.at(dir).second.second;

      if ((activationCheck(toff, tofs))
         && (inLineCheck(toff, tofs))
         && (!passedCheck(toff, tofs)))
      {
         res = { toff, tofs };
         return true;
      }
      if ((activationCheck(tosf, toss))
         && (inLineCheck(tosf, toss))
         && (!passedCheck(tosf, toss)))
      {
         res = { tosf, toss };
         return true;
      }
      return false;
   };

   auto checkFindNextPoint = [pointCheck, &passedFlagMtx](size_t& dirChanges, SVCG::route_point& candidate, TraversalDirection& dir, SVCG::route_point current)
   {
      SVCG::route_point subcandidate;
      while (!pointCheck(subcandidate, dir, current.row, current.col) && dirChanges < 4)
      {
         dir = nextStepRule.at(dir);
         dirChanges++;
      }
      candidate = subcandidate;
   };

   std::vector<std::vector<SVCG::route_point>> isoLineVct;
   // Построение контура
   for (size_t rIdx = 0; rIdx < actValMtx->GetRowCount(); rIdx++)
   {
      auto& row = rawdata.arr[rIdx];
      for (size_t cIdx = 0; cIdx < actValMtx->GetColCount(); cIdx++)
      {
         if (activationCheck(rIdx, cIdx) && inLineCheck(rIdx, cIdx) && !passedCheck(rIdx, cIdx))
         {
            TraversalDirection dir = TraversalDirection::TD_U;
            std::vector<SVCG::route_point> resultList;
            SVCG::route_point start{ static_cast<int>(rIdx), static_cast<int>(cIdx) }, cur;
            resultList.push_back(start);
            dir = nextPointPickRule.at(dir);
            cur = start;
            size_t dirChanges = 0;
            SVCG::route_point candidate;
            checkFindNextPoint(dirChanges, candidate, dir, cur);
            if (dirChanges >= 4)
               break;
            cur = candidate;
            passedFlagMtx->Set(cur.row, cur.col, true);
            while (cur.row != start.row || cur.col != start.col)
            {
               dirChanges = 0;
               checkFindNextPoint(dirChanges, candidate, dir, cur);
               ATLASSERT(dirChanges < 4);
               resultList.push_back(candidate);
               cur = candidate;
               passedFlagMtx->Set(cur.row, cur.col, true);
            }
            isoLineVct.emplace_back(resultList);
         }
      }
   }

   std::vector<math::geo_points> isoLineGeoVct;
   for (auto& line : isoLineVct)
   {
      math::geo_points geoline;
      for (auto& point : line)
         geoline.emplace_back(SVCG::RoutePointToPositionPoint(point));
      isoLineGeoVct.emplace_back(geoline);
   }

   addChartObjectSet(isoLineGeoVct, height, H);
}

// NOTE: минимальное покрытие скипает очень много точек...
void IsolineGenerator::generateRC2(const converter::raw_data_ref& rawdata, double height, int H)
{
   const auto& envstt = GetPack()->settings->env_stt;
   std::vector<colreg::geo_point> isoPoints;
   constexpr double eps = 0.0001;

   for (size_t rIdx = 0; rIdx < rawdata.size - 1; rIdx++)
   {
      auto& rowHigh = rawdata.arr[rIdx + 1];
      auto& rowLow = rawdata.arr[rIdx];
      for (size_t cIdx = 0; cIdx < rowHigh.size - 1; cIdx++)
      {
         // NOTE: нули расположены снизу слева
         int iCIdx = static_cast<int>(cIdx);
         int iRIdx = static_cast<int>(rIdx);
         colreg::geo_point gplu = SVCG::RoutePointToPositionPoint(SVCG::route_point{ iRIdx + 1, iCIdx }, envstt);
         colreg::geo_point gpll = SVCG::RoutePointToPositionPoint(SVCG::route_point{ iRIdx, iCIdx }, envstt);
         colreg::geo_point gpru = SVCG::RoutePointToPositionPoint(SVCG::route_point{ iRIdx + 1, iCIdx + 1 }, envstt);
         colreg::geo_point gprl = SVCG::RoutePointToPositionPoint(SVCG::route_point{ iRIdx, iCIdx + 1 }, envstt);
         double hlu = rowHigh.arr[cIdx];
         double hll = rowLow.arr[cIdx];
         double hru = rowHigh.arr[cIdx + 1];
         double hrl = rowLow.arr[cIdx + 1];
         auto checkPoint = [eps, &isoPoints](colreg::geo_point pt)->bool
         {
            return (std::find_if(isoPoints.begin(), isoPoints.end(), [pt, eps](const colreg::geo_point& rs) { return math::isEqual(rs, pt, eps); }) == isoPoints.end());
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
               colreg::geo_point gp = gpll;
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
               colreg::geo_point gp = gplu;
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
               colreg::geo_point gp = gpru;
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
               colreg::geo_point gp = gplu;
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
   std::vector<std::vector<colreg::geo_point>> isoLineVct;
   //isoLineVct.emplace_back();
   //for (auto& elem : isoPoints)
   //   isoLineVct.back().emplace_back(elem);
   auto nearestPointSetComparator = [](const std::vector<colreg::geo_point>& check, const std::vector<colreg::geo_point>& nearest, math::geo_point isoPoint, double& nearestMinDist) -> bool
   {
      auto isocheckmin = std::min_element(check.begin(), check.end(),
         [&isoPoint](const colreg::geo_point& ptCheck, const colreg::geo_point& nearest) -> bool
         {
            return math::distance(ptCheck, isoPoint) < math::distance(nearest, isoPoint);
         }
      );
      double checkDist = (isocheckmin == check.end() ? 0. : math::distance(*isocheckmin, isoPoint));
      auto isosmallestmin = std::min_element(nearest.begin(), nearest.end(),
         [&isoPoint](const colreg::geo_point& ptCheck, const colreg::geo_point& nearest) -> bool
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
         [&isoPoint, &nearestMinDist, nearestPointSetComparator](const std::vector<colreg::geo_point>& check, const std::vector<colreg::geo_point>& nearest)->bool
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
                  [&isoPoint](const colreg::geo_point& ptCheck, const colreg::geo_point& nearest) -> bool
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

      auto cmp = [](colreg::geo_point a, colreg::geo_point b)->bool
      {
         return a.lon < b.lon || a.lon == b.lon && a.lat < b.lat;
      };

      auto cw = [](colreg::geo_point a, colreg::geo_point b, colreg::geo_point c)->bool
      {
         return a.lon * (b.lat - c.lat) + b.lon * (c.lat - a.lat) + c.lon * (a.lat - b.lat) < 0;
      };

      auto ccw = [](colreg::geo_point a, colreg::geo_point b, colreg::geo_point c)->bool
      {
         return a.lon * (b.lat - c.lat) + b.lon * (c.lat - a.lat) + c.lon * (a.lat - b.lat) > 0;
      };

      auto convex_hull = [cmp, cw, ccw](std::vector<colreg::geo_point>& a)
      {
         if (a.size() == 1)  return;
         std::sort(a.begin(), a.end(), cmp);
         colreg::geo_point p1 = a[0], p2 = a.back();
         std::vector<colreg::geo_point> up, down;
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

   addChartObjectSet(isoLineVct, height, H);
}


void IsolineGenerator::addChartObjectSet(const std::vector<math::geo_points>& data, double height, int H)
{
   for (auto& isoLine : data)
   {
      chart_storage& isoLineStorageCell = m_genNewObjectRef();
      isoLineStorageCell.type = colreg::OBJECT_TYPE::OT_ISOLINE;
      isoLineStorageCell.geom_contour_vct.emplace_back();
      // TODO: подкорректировать цвет
      int rgb[3];
      HSVtoRGB(H, 1., 1., rgb);
      char color[64];
      sprintf(color, "%i %i %i", rgb[0], rgb[1], rgb[2]);
      isoLineStorageCell.prop_holder_vct.emplace_back("Color", color);
      isoLineStorageCell.prop_holder_vct.emplace_back("Depth", std::to_string(height).c_str());
      for (auto& elem : isoLineStorageCell.prop_holder_vct)
         isoLineStorageCell.prop_vct.emplace_back(elem.key.c_str(), elem.val.c_str());
      auto& isoLineGeom = isoLineStorageCell.geom_contour_vct.back();
      for (auto& point : isoLine)
         isoLineGeom.emplace_back(point);
      isoLineStorageCell.Commit();

      m_addObject(isoLineStorageCell);
   }
}