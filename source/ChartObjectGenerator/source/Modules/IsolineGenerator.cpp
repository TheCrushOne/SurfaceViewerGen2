#include "stdafx.h"
#include "IsolineGenerator.h"
#include "math/math_utils.h"
#include "SVCG/positioning.h"

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
   for (size_t levelIdx = 0; levelIdx < levelCount; levelIdx++)
      generateIsolineLevel(rawdata, min + step * static_cast<double>(levelIdx), 360. / static_cast<double>(levelCount)* static_cast<double>(levelIdx));
}

void IsolineGenerator::generateIsolineLevel(const converter::raw_data_ref& rawdata, double height, int H)
{
   const auto& envstt = GetPack()->settings->env_stt;
   std::vector<colreg::geo_point> isoPoints;

   for (size_t rIdx = 0; rIdx < rawdata.size - 1; rIdx++)
   {
      auto& rowHigh = rawdata.arr[rIdx];
      auto& rowLow = rawdata.arr[rIdx + 1];
      for (size_t cIdx = 0; cIdx < rowHigh.size - 1; cIdx++)
      {
         colreg::geo_point gplu = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx), static_cast<int>(cIdx) }, envstt);
         colreg::geo_point gpll = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx + 1), static_cast<int>(cIdx) }, envstt);
         colreg::geo_point gpru = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx), static_cast<int>(cIdx + 1) }, envstt);
         colreg::geo_point gprl = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx + 1) , static_cast<int>(cIdx + 1) }, envstt);
         if ((height - rowHigh.arr[cIdx]) * (height - rowHigh.arr[cIdx + 1]) < 0)
         {
            colreg::geo_point gp = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx), 0 }, envstt);
            double heightPerLat = (rowHigh.arr[cIdx + 1] - rowHigh.arr[cIdx]) / (gpru.lat - gplu.lat);
            gp.lat = (rowHigh.arr[cIdx + 1] - height) / heightPerLat + gplu.lat;
            if (std::find(isoPoints.begin(), isoPoints.end(), gp) == isoPoints.end())
               isoPoints.emplace_back(gp);
         }
         if ((height - rowHigh.arr[cIdx]) * (height - rowLow.arr[cIdx]) < 0)
         {
            colreg::geo_point gp = SVCG::RoutePointToPositionPoint(SVCG::route_point{ 0, static_cast<int>(cIdx) }, envstt);
            double heightPerLon = (rowLow.arr[cIdx] - rowHigh.arr[cIdx]) / (gpll.lon - gplu.lon);
            gp.lon = (rowLow.arr[cIdx] - height) / heightPerLon + gplu.lon;
            if (std::find(isoPoints.begin(), isoPoints.end(), gp) == isoPoints.end())
               isoPoints.emplace_back(gp);
         }
         if ((height - rowHigh.arr[cIdx + 1]) * (height - rowLow.arr[cIdx + 1]) < 0)
         {
            colreg::geo_point gp = SVCG::RoutePointToPositionPoint(SVCG::route_point{ 0, static_cast<int>(cIdx + 1) }, envstt);
            double heightPerLon = (rowLow.arr[cIdx + 1] - rowHigh.arr[cIdx + 1]) / (gprl.lon - gpru.lon);
            gp.lon = (rowLow.arr[cIdx + 1] - height) / heightPerLon + gpru.lon;
            if (std::find(isoPoints.begin(), isoPoints.end(), gp) == isoPoints.end())
               isoPoints.emplace_back(gp);
         }
         if ((height - rowLow.arr[cIdx]) * (height - rowLow.arr[cIdx + 1]) < 0)
         {
            colreg::geo_point gp = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx + 1), 0 }, envstt);
            double heightPerLat = (rowLow.arr[cIdx + 1] - rowLow.arr[cIdx]) / (gprl.lat - gpll.lat);
            gp.lat = (rowLow.arr[cIdx + 1] - height) / heightPerLat + gpll.lat;
            if (std::find(isoPoints.begin(), isoPoints.end(), gp) == isoPoints.end())
               isoPoints.emplace_back(gp);
         }
      }
   }

   // NOTE: Распределение точек по изолиниям
   // WARNING: костыль!!!
   auto pt1 = SVCG::RoutePointToPositionPoint({ 0, 0 }, envstt);
   auto pt2 = SVCG::RoutePointToPositionPoint({ 1, 1 }, envstt);
   m_maxRadius = math::distance(pt2, pt1);
   std::vector<std::vector<colreg::geo_point>> isoLineVct;
   for (size_t isoIdx = 0; isoIdx < isoPoints.size(); isoIdx++)
   {
      auto& isoPoint = isoPoints.at(isoIdx);
      double min = std::numeric_limits<double>::max();
      double nearestMinDist = std::numeric_limits<double>::max();
      auto nearestIsoLine = std::min_element(isoLineVct.begin(), isoLineVct.end(),
         [&nearestMinDist, isoPoint](const std::vector<colreg::geo_point>& check, const std::vector<colreg::geo_point>& smallest) -> bool
         {
            auto isocheckmin = std::min_element(check.begin(), check.end(), [isoPoint](const colreg::geo_point& ptCheck, const colreg::geo_point& smallest) -> bool { return math::distance(ptCheck, isoPoint) < math::distance(smallest, isoPoint); });
            double checkDist = isocheckmin == check.end() ? 0. : math::distance(*isocheckmin, isoPoint);
            auto isosmallestmin = std::min_element(smallest.begin(), smallest.end(), [isoPoint](const colreg::geo_point& ptCheck, const colreg::geo_point& smallest) -> bool { return math::distance(ptCheck, isoPoint) < math::distance(smallest, isoPoint); });
            double smallestDist = isosmallestmin == smallest.end() ? 0. : math::distance(*isosmallestmin, isoPoint);
            ATLASSERT(checkDist != 0. && smallestDist != 0.);
            if (checkDist < smallestDist)
            {
               nearestMinDist = checkDist;
               return true;
            }
            else
            {
               nearestMinDist = smallestDist;
               return false;
            }
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

   // NOTE: построение изолиний
   for (auto& pointList : isoLineVct)
   {
      std::vector<std::pair<colreg::geo_point, colreg::geo_point>> vectorList;
      for (auto& srcPoint : pointList)
      {
         for (auto& dstPoint : pointList)
         {
            bool distOk = math::distance(srcPoint, dstPoint) <= m_maxRadius;
            auto res = std::find_if(vectorList.begin(), vectorList.end(),
               [srcPoint, dstPoint](const std::pair<colreg::geo_point, colreg::geo_point>& elem) -> bool
               {
                  return ((srcPoint == elem.first && dstPoint == elem.second)
                     || (srcPoint == elem.second && dstPoint == elem.first));
               }
            );
            if (distOk && srcPoint != dstPoint && res == vectorList.end())
               vectorList.emplace_back(std::pair<colreg::geo_point, colreg::geo_point>{ srcPoint, dstPoint });
            //else
               //GetPack()->comm->Message(ICommunicator::MessageType::MS_Error, "check failed %f %f -> %f %f [%i, %i, %i]", srcPoint.lat, srcPoint.lon, dstPoint.lat, dstPoint.lon, distOk, srcPoint != dstPoint, res == vectorList.end());
         }
      }

      std::sort(vectorList.begin(), vectorList.end(),
         [](std::pair<colreg::geo_point, colreg::geo_point>& check, std::pair<colreg::geo_point, colreg::geo_point>& smallest) -> bool
         {
            return math::distance(check.first, check.second) < math::distance(smallest.first, smallest.second);
         }
      );

      auto jointCounter = [vectorList](const colreg::geo_point& point) -> size_t
      {
         size_t count = 0;
         for (auto& vector : vectorList)
         {
            if (vector.first == point || vector.second == point)
               count++;
         }
         return count;
      };
      auto startPoint = std::min_element(pointList.begin(), pointList.end(),
         [jointCounter](const colreg::geo_point& ptCheck, const colreg::geo_point& smallest) -> bool
         {
            return jointCounter(ptCheck) < jointCounter(smallest);
         }
      );
      colreg::route_point sp = *startPoint;
      pointList.clear();
      pointList.emplace_back(sp);
      bool inRadiusExists = true;
      while (inRadiusExists)
      {
         auto& last = pointList.back();
         // NOTE: по идее, в сортированном массиве будет найдено минимальное ребро
         auto nearestJointVec = std::find_if(vectorList.begin(), vectorList.end(),
            [last](const std::pair<math::geo_point, math::geo_point>& check) -> bool
            {
               return last == check.first || last == check.second;
            }
         );
         if (nearestJointVec != vectorList.end())
         {
            pointList.emplace_back(last == nearestJointVec->first ? nearestJointVec->second : nearestJointVec->first);
            vectorList.erase(nearestJointVec);
         }
         else
            inRadiusExists = false;
      }
   }

   for (auto& isoLine : isoLineVct)
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
