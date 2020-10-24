#include "stdafx.h"
#include "rulertool.h"
//#include "Colreg\simulator.h"
//#include "Selection\SelectetObjectManager.h"

using namespace SV;
using namespace SV::user_interface;

void RulerTool::MouseMove(CPoint point)
{
   if (!_renderer)
      return;

   if (_lbDown)
   {
      renderRuler(_clickPos, point);
      InvalidateView();
   }
   BaseTool::MouseMove(point);

}


void RulerTool::LmouseDown(CPoint point)
{
   BaseTool::LmouseDown(point);
}

void RulerTool::LmouseUp(CPoint point)
{

   InvalidateView();

   BaseTool::LmouseUp(point);
}

void RulerTool::MmouseDown(CPoint point)
{

}
void RulerTool::MmouseUp(CPoint point)
{

}
void RulerTool::RmouseDown(CPoint point)
{
   user_interface::SetEditMode(EDIT_MODE::EM_DRAG);
}
void RulerTool::RmouseUp(CPoint point)
{

}
void RulerTool::MouseLeave()
{

}

std::string RulerTool::getDistText(double dist)const
{
   std::stringstream s;
   s.precision(2);
   if (dist > 1.)
      s << dist << " NM";
   else
      s << dist * 10 << " kb";
   return s.str();
}

void RulerTool::renderRuler(CPoint from, CPoint to) const
{
   const auto geoFrom = _renderer->PixelToGeo(math::point{ (double)from.y, (double)from.x });
   const auto geoTo = _renderer->PixelToGeo(math::point{ (double)to.y, (double)to.x });

   double dist;
   double dir;
   math::distance_direction(geoFrom, geoTo, dist, dir);
   if (dist < METERS_TO_MILE)
      return;

   _renderer->AddObject({ {geoFrom, geoTo}, {1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(255, 255, 0), ""} });
   _renderer->AddObject({ {geoFrom, geoTo}, {1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NULL, RGB(255, 255, 0), "", 0, 0, 25} });


   const auto distSection = math::distance(geoFrom, _renderer->PixelToGeo(math::point{ (double)from.y + 3, (double)from.x }));

   for (auto step = 0.; step <= dist; step += dist * 0.1)
   {
      const auto p = math::calc_point(geoFrom, step, dir);
      const auto p1 = math::calc_point(p, distSection, dir + 90);
      const auto p2 = math::calc_point(p, distSection, dir - 90);

      _renderer->AddObject({ {p1, p2}, {1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(0, 0, 200), ""} });
   }

   std::stringstream s;
   s.width(4);
   s.precision(4);

   s << getDistText(dist) << ", " << dir << " ° ";
   _renderer->AddObject({ {geoTo}, {3, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(0, 0, 250), s.str(), 0, 25} });
}