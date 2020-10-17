#include "stdafx.h"
#include "CoverageGenerator.h"
#include "math/math_utils.h"
#include "SVCG/positioning.h"
#include "common/chart_object.h"

using namespace SV;
using namespace SV::chart_object;

void CoverageGenerator::GenerateChartBorder(const pathfinder::GeoMatrix& rawdata, chart_object_unit& border)
{
   const auto& envstt = GetService()->GetSettingsSerializerHolder()->GetSettings().env_stt;
   border.type = OBJECT_TYPE::OT_BORDER_AREA;
   int rowCount = rawdata.GetRowCount(), colCount = rawdata.GetColCount();
   border.geom_contour_vct.emplace_back();
   auto& borderCnt = border.geom_contour_vct.back();
   borderCnt.emplace_back(transfercase::RoutePointToPositionPoint(CG::route_point{ 0, 0 }, envstt));
   borderCnt.emplace_back(transfercase::RoutePointToPositionPoint(CG::route_point{ rowCount - 1, 0 }, envstt));
   borderCnt.emplace_back(transfercase::RoutePointToPositionPoint(CG::route_point{ rowCount - 1, colCount - 1 }, envstt));
   borderCnt.emplace_back(transfercase::RoutePointToPositionPoint(CG::route_point{ 0, colCount - 1 }, envstt));
   // NOTE: Первая точка повторяется для закольцованности контура
   borderCnt.emplace_back(transfercase::RoutePointToPositionPoint(CG::route_point{ 0, 0 }, envstt));
}