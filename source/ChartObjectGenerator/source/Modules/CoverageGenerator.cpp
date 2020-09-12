#include "stdafx.h"
#include "CoverageGenerator.h"
#include "math/math_utils.h"
#include "SVCG/positioning.h"

using namespace chart_object;

void CoverageGenerator::GenerateChartBorder(const pathfinder::GeoMatrix* rawdata, chart_storage& border)
{
   //const auto& envstt = GetPack()->settings->env_stt;
   settings::environment_settings envstt;
   border.type = colreg::OBJECT_TYPE::OT_BORDER_AREA;
   int rowCount = rawdata->GetRowCount(), colCount = rawdata->GetColCount();
   border.geom_contour_vct.emplace_back();
   auto& borderCnt = border.geom_contour_vct.back();
   borderCnt.emplace_back(SVCG::RoutePointToPositionPoint(SVCG::route_point{ 0, 0 }, envstt));
   borderCnt.emplace_back(SVCG::RoutePointToPositionPoint(SVCG::route_point{ rowCount - 1, 0 }, envstt));
   borderCnt.emplace_back(SVCG::RoutePointToPositionPoint(SVCG::route_point{ rowCount - 1, colCount - 1 }, envstt));
   borderCnt.emplace_back(SVCG::RoutePointToPositionPoint(SVCG::route_point{ 0, colCount - 1 }, envstt));
   // NOTE: Первая точка повторяется для закольцованности контура
   borderCnt.emplace_back(SVCG::RoutePointToPositionPoint(SVCG::route_point{ 0, 0 }, envstt));
   //border.Commit();

   //m_addObject(border);
}