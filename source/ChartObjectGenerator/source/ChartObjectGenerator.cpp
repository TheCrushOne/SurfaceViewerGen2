#include "stdafx.h"
#include "ChartObjectGenerator.h"
#include "SVCG/positioning.h"
#include "math/math_utils.h"
#include <algorithm>
#include <limits>

using namespace chart_object;

ChartObjectGenerator::ChartObjectGenerator()
   : Communicable(nullptr)
   , m_settings(nullptr)
{}

bool ChartObjectGenerator::Init(ICommunicator* comm, settings::environment_settings* envStt)
{
   m_settings = envStt;
   SetCommunicator(comm);
   return true;
}

bool ChartObjectGenerator::GenerateStatic(const converter::raw_data_ref& rawdata)
{
   if (m_lock)
      return false;
   prepareLocalStorage();
   generateChartBorder(rawdata);
   generateIsolines(rawdata);
   return true;
}

void ChartObjectGenerator::prepareRef() const
{
   const_cast<ChartObjectGenerator*>(this)->m_chartObjectRef = colreg::chart_objects_ref(m_chartObjVct.data(), m_chartObjVct.size());
}

void ChartObjectGenerator::prepareLocalStorage()
{
   m_chartObjVct.clear();
   m_chartStorage.clear();
}

void ChartObjectGenerator::addChartObject(chart_storage& storage)
{
   static colreg::chart_object_id id = 0;
   colreg::object_props_ref propRef = colreg::object_props_ref(storage.prop_vct.data(), storage.prop_vct.size());
   colreg::object_geometry_ref geoRef = colreg::object_geometry_ref(storage.geom_contour_ref_vct.data(), storage.geom_contour_ref_vct.size());
   m_chartObjVct.emplace_back(colreg::chart_object(id++, storage.type, geoRef, propRef));
}

void ChartObjectGenerator::generateChartBorder(const converter::raw_data_ref& rawdata)
{
   chart_storage& border = generateNew();
   border.type = colreg::OBJECT_TYPE::OT_BORDER_AREA;
   int rowCount = rawdata.size, colCount = rawdata.size ? rawdata.arr[0].size : 0;
   border.geom_contour_vct.emplace_back();
   auto& borderCnt = border.geom_contour_vct.back();
   borderCnt.emplace_back(SVCG::RoutePointToPositionPoint(SVCG::route_point{ 0, 0 }, *m_settings));
   borderCnt.emplace_back(SVCG::RoutePointToPositionPoint(SVCG::route_point{ rowCount - 1, 0 }, *m_settings));
   borderCnt.emplace_back(SVCG::RoutePointToPositionPoint(SVCG::route_point{ rowCount - 1, colCount - 1 }, *m_settings));
   borderCnt.emplace_back(SVCG::RoutePointToPositionPoint(SVCG::route_point{ 0, colCount - 1 }, *m_settings));
   // NOTE: ������ ����� ����������� ��� ���������������� �������
   borderCnt.emplace_back(SVCG::RoutePointToPositionPoint(SVCG::route_point{ 0, 0 }, *m_settings));
   border.Commit();

   addChartObject(border);
}

void ChartObjectGenerator::generateIsolines(const converter::raw_data_ref& rawdata)
{
   std::vector<colreg::geo_point> isoPoints;
   //double height = 27.;
   double height = 50.;
   for (size_t rIdx = 0; rIdx < rawdata.size - 1; rIdx++)
   {
      auto& rowHigh = rawdata.arr[rIdx];
      auto& rowLow = rawdata.arr[rIdx + 1];
      for (size_t cIdx = 0; cIdx < rowHigh.size - 1; cIdx++)
      {
         colreg::geo_point gplu = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx), static_cast<int>(cIdx) }, *m_settings);
         colreg::geo_point gpll = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx + 1), static_cast<int>(cIdx) }, *m_settings);
         colreg::geo_point gpru = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx), static_cast<int>(cIdx + 1) }, *m_settings);
         colreg::geo_point gprl = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx + 1) , static_cast<int>(cIdx + 1) }, *m_settings);
         if ((height - rowHigh.arr[cIdx]) * (height - rowHigh.arr[cIdx + 1]) < 0)
         {
            colreg::geo_point gp = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx), 0 }, *m_settings);
            double heightPerLat = (rowHigh.arr[cIdx + 1] - rowHigh.arr[cIdx]) / (gpru.lat - gplu.lat);
            gp.lat = (rowHigh.arr[cIdx + 1] - height) / heightPerLat + gplu.lat;
            if (std::find(isoPoints.begin(), isoPoints.end(), gp) == isoPoints.end())
               isoPoints.emplace_back(gp);
         }
         if ((height - rowHigh.arr[cIdx]) * (height - rowLow.arr[cIdx]) < 0)
         {
            colreg::geo_point gp = SVCG::RoutePointToPositionPoint(SVCG::route_point{ 0, static_cast<int>(cIdx) }, *m_settings);
            double heightPerLon = (rowLow.arr[cIdx] - rowHigh.arr[cIdx]) / (gpll.lon - gplu.lon);
            gp.lon = (rowLow.arr[cIdx] - height) / heightPerLon + gplu.lon;
            if (std::find(isoPoints.begin(), isoPoints.end(), gp) == isoPoints.end())
               isoPoints.emplace_back(gp);
         }
         if ((height - rowHigh.arr[cIdx + 1]) * (height - rowLow.arr[cIdx + 1]) < 0)
         {
            colreg::geo_point gp = SVCG::RoutePointToPositionPoint(SVCG::route_point{ 0, static_cast<int>(cIdx + 1) }, *m_settings);
            double heightPerLon = (rowLow.arr[cIdx + 1] - rowHigh.arr[cIdx + 1]) / (gprl.lon - gpru.lon);
            gp.lon = (rowLow.arr[cIdx + 1] - height) / heightPerLon + gpru.lon;
            if (std::find(isoPoints.begin(), isoPoints.end(), gp) == isoPoints.end())
               isoPoints.emplace_back(gp);
         }
         if ((height - rowLow.arr[cIdx]) * (height - rowLow.arr[cIdx + 1]) < 0)
         {
            colreg::geo_point gp = SVCG::RoutePointToPositionPoint(SVCG::route_point{ static_cast<int>(rIdx + 1), 0 }, *m_settings);
            double heightPerLat = (rowLow.arr[cIdx + 1] - rowLow.arr[cIdx]) / (gprl.lat - gpll.lat);
            gp.lat = (rowLow.arr[cIdx + 1] - height) / heightPerLat + gpll.lat;
            if (std::find(isoPoints.begin(), isoPoints.end(), gp) == isoPoints.end())
               isoPoints.emplace_back(gp);
         }
      }
   }

   // WARNING: �������!!!
   auto pt1 = SVCG::RoutePointToPositionPoint({0, 0}, *m_settings);
   auto pt2 = SVCG::RoutePointToPositionPoint({2, 2}, *m_settings);
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
      // NOTE: ��������� ����� � �������� ��������� �� ��������
      auto isoLineIter = std::min_element(nearestIsoLine->begin(), nearestIsoLine->end(),
         [isoPoint](const colreg::geo_point& ptCheck, const colreg::geo_point& smallest) -> bool
         { 
            return math::distance(ptCheck, isoPoint) < math::distance(smallest, isoPoint);
         }
      );
      if (isoLineIter == nearestIsoLine->end())
         nearestIsoLine->emplace_back(isoPoint);
      else
         nearestIsoLine->insert(isoLineIter, isoPoint);
   }

   for (auto& isoLine : isoLineVct)
   {
      isoSort(isoLine);
      chart_storage& isoLineStorageCell = generateNew();
      isoLineStorageCell.type = colreg::OBJECT_TYPE::OT_ISOLINE;
      isoLineStorageCell.geom_contour_vct.emplace_back();
      auto& isoLineGeom = isoLineStorageCell.geom_contour_vct.back();
      for (auto& point : isoLine)
         isoLineGeom.emplace_back(point);
      isoLineStorageCell.Commit();

      addChartObject(isoLineStorageCell);
   }
}

void ChartObjectGenerator::isoSort(std::vector<colreg::geo_point>& isoline)
{
   if (isoline.empty())
      return;
   std::vector<colreg::geo_point> sorted;
   
   std::vector<short> inRadiusCountVct;
   inRadiusCountVct.resize(isoline.size());
   for (size_t idx = 0; idx < isoline.size(); idx++)
   {
      for (auto& elem : isoline)
      {
         if (math::distance(elem, isoline.at(idx)) < m_maxRadius)
            inRadiusCountVct[idx]++;
      }
   }
   auto startPointIter = std::find_if(inRadiusCountVct.begin(), inRadiusCountVct.end(), [](short val) -> bool { return val < 2; });
   sorted.emplace_back(startPointIter != inRadiusCountVct.end() ? isoline.at(std::distance(startPointIter, inRadiusCountVct.begin())) : isoline.back());
   while (!isoline.empty())
   {
      auto& checker = sorted.back();
      auto nearest = std::min_element(isoline.begin(), isoline.end(),
         [checker](const colreg::geo_point& check, const colreg::geo_point& smallest)->bool
         {
            return math::distance(check, checker) < math::distance(smallest, checker);
         }
      );
      sorted.emplace_back(*nearest);
      isoline.erase(nearest);
   }
   isoline = sorted;
}

void ChartObjectGenerator::generateNoGoAreas(const converter::raw_data_ref& rawdata)
{

}

void ChartObjectGenerator::generateNoFlyAreas(const converter::raw_data_ref& rawdata)
{

}

chart_object::iGenerator * CreateGenerator()
{
   return new chart_object::ChartObjectGenerator();
}