#include "stdafx.h"
#include "NetLayer.h"

namespace
{
   NetLayer layer;
}
void NetLayer::createNet(render::iRender*renderer)
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
         renderer->AddObject({ { p1,p2 }, {1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(200, 200, 200), ""} });
   }
   for (auto y = 0.; y <= height; y += _step)
   {
      const auto p1 = math::calc_point(geoLeftTop, y, 180);
      const auto p2 = math::calc_point(p1, width, 90.);
      renderer->AddObject({ { p1,p2 }, {1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(200, 200, 200), ""} });
   }
}