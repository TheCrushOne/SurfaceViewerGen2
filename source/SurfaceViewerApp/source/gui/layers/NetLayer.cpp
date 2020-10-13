#include "stdafx.h"
#include "NetLayer.h"
#include "simulator\simulator.h"
#include "SVCG/positioning.h"

namespace
{
   NetLayer layer;
}
void NetLayer::createNet(render::iRender* renderer)
{
   const auto size = renderer->GetSize();
   const auto geoLeftTop = renderer->PixelToGeo({ 0., 0. });
   const auto geoLeftBottom = renderer->PixelToGeo({ (double)size.cy, 0. });
   const auto geoRightBottom = renderer->PixelToGeo({ (double)size.cy, (double)size.cx });
   
   const auto height = math::distance(geoLeftTop, geoLeftBottom);
   const auto width = math::distance(geoLeftBottom, geoRightBottom);

   for (auto x = 0.; x <= width; x += _step)
   {
      const auto p1 = math::calc_point(geoLeftTop, x, 90.);
      const auto p2 = math::calc_point(p1, height, 180.);
      renderer->AddObject({ { p1,p2 }, {1, render::LINE_STYLE::LL_DASH_DOT, render::FILL_TYPE::FT_NONE, RGB(200, 200, 200), ""} });
   }
   for (auto y = 0.; y <= height; y += _step)
   {
      const auto p1 = math::calc_point(geoLeftTop, y, 180);
      const auto p2 = math::calc_point(p1, width, 90.);
      renderer->AddObject({ { p1,p2 }, {1, render::LINE_STYLE::LL_DASH_DOT, render::FILL_TYPE::FT_NONE, RGB(200, 200, 200), ""} });
   }
}

void NetLayer::createCells(render::iRender* renderer)
{
   const auto* sim = simulator::getSimulator();
   if (!sim)
      return;
   const auto& simulationState = sim->GetState();
   const auto& envStt = sim->GetAppSettings().env_stt;
   auto& meta = simulationState.GetChartGridMeta();
   auto gridLight = RGB(200, 200, 200);

   // NOTE: случай не загруженного или пустого сценария
   if (meta.row_count == 0 || meta.col_count == 0)
      return;

   for (size_t rowIdx = 1; rowIdx < meta.row_count - 1; rowIdx++)
   {
      const auto p1 = SVCG::RoutePointToPositionPoint({ static_cast<int>(rowIdx), 0 }, envStt);
      const auto p2 = SVCG::RoutePointToPositionPoint({ static_cast<int>(rowIdx), static_cast<int>(meta.col_count) - 1 }, envStt);
      renderer->AddObject({ { p1,p2 }, {1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, gridLight, ""} });
   }
   for (size_t colIdx = 1; colIdx < meta.col_count - 1; colIdx++)
   {
      const auto p1 = SVCG::RoutePointToPositionPoint({ 0, static_cast<int>(colIdx) }, envStt);
      const auto p2 = SVCG::RoutePointToPositionPoint({ static_cast<int>(meta.row_count) - 1, static_cast<int>(colIdx) }, envStt);
      renderer->AddObject({ { p1,p2 }, {1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, gridLight, ""} });
   }
}