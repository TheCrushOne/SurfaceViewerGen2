#include "stdafx.h"
#include "TrackLayerHelper.h"
#include "gui\user_interface.h"
#include "colreg/ChartSafetyStructs.h"

namespace
{
   bool is_non_default_scales(const colreg::domain_scales& scales)
   {
      return (scales.forward_scale != 1.0 && scales.forward_scale != colreg::NO_VALUE) ||
         (scales.left_scale != 1.0 && scales.left_scale != colreg::NO_VALUE) ||
         (scales.backward_scale != 1.0 && scales.backward_scale != colreg::NO_VALUE) ||
         (scales.forward_scale != 1.0 && scales.forward_scale != colreg::NO_VALUE);
   }
}
void TrackLayerHelper::RenderShiptrack(render::iRender* renderer, const colreg::track_full_info_ref* track, const render::object_info& info, int trackLinesIntervalSeconds, bool renderXTE)
{
   if (!track || !track->size)
      return;
   double lastTrackLineTime = track->points[0].time;
   for (size_t i = 0; i < track->size; ++i)
   {
      renderer->AddObject({ {track->points[i].pos}, info });

      if (/*track->chart_context && */(i < (track->size - 1)))
      {
         // TODO: refact
         auto newInfo = info;
         unsigned long color = 0;
         if (color)
         {
            newInfo.color = color;
            newInfo.alpha = 100;
            newInfo.width += 2;
            renderer->AddObject({ {track->points[i].pos}, newInfo });
         }
      }

      if (track->domain_border && i && (i < (track->size - 1)))
      {
         render::object_info newInfo;

         newInfo.alpha = 100;
         newInfo.color = RGB(34, 177, 76);

         newInfo.width = 2;

         renderer->AddObject({ { track->domain_border[i - 1].left, track->domain_border[i].left }, newInfo });
         renderer->AddObject({ { track->domain_border[i - 1].right, track->domain_border[i].right }, newInfo });

      }

      if (trackLinesIntervalSeconds > 0 && i && (track->points[i].time - lastTrackLineTime + 1) >= trackLinesIntervalSeconds)
      {
         const auto dist = math::distance(renderer->PixelToGeo(math::point{}), renderer->PixelToGeo(math::point{ 5., 0. }));
         auto course = track->points[i].course != colreg::NO_VALUE ? track->points[i].course : track->points[i].heading;
         auto ptLeft = math::calc_point(track->points[i].pos, dist, course - 90);
         auto ptRight = math::calc_point(track->points[i].pos, dist, course + 90);
         renderer->AddObject({ {ptLeft, ptRight}, info });
         lastTrackLineTime = track->points[i].time;
      }

      if (renderXTE && i < track->size - 1)
      {
         auto xteInfo = info;
         xteInfo.width = 5;
         xteInfo.color = RGB(200, 0, 0);
         const auto course = track->points[i].course;
         renderer->AddObject({ {math::calc_point(track->points[i].pos, track->points[i].left_XTE, course - 90)}, xteInfo });
         xteInfo.color = RGB(0, 0, 200);
         renderer->AddObject({ {math::calc_point(track->points[i].pos, track->points[i].right_XTE, course + 90)}, xteInfo });
      }
   }
}


void TrackLayerHelper::renderRoute(render::iRender* renderer, colreg::id_type id, const colreg::route_ref* route, const render::object_info& info, ColregSimulation::ROUTE_TYPE type)
{
   using namespace colreg;
   if (!route || route->size < 2)
      return;
   const auto ft = type == ColregSimulation::ROUTE_TYPE::RT_SUB_OPTIMAL ? render::FIND_TYPE::FT_NONE : render::FIND_TYPE::FT_FIND_FAST;
   user_interface::user_info ui;
   ui.data = 0;


   for (size_t i = 0; i < route->size - 1; i++)
   {
      const route_point& rp = route->arr[i];
      const route_point& rpNext = route->arr[i + 1];

      ui.type = (char)type;
      ui.index = (short)i;

      renderRouteSegment(renderer, id, ui.value, rp, rpNext, info, type);

      std::stringstream s;  s << i;
      renderer->AddObject({ {rp.pos}
                              , { 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, info.color, s.str().c_str()}
                                 , {ft, id, render::FIND_OBJECT_TYPE::FOT_ROUTE_POINT, ui.value } });

   }
   ui.type = (char)type;
   ui.index = (short)(route->size - 1);
   std::stringstream s;  s << route->size - 1;
   renderer->AddObject({ { route->arr[route->size - 1].pos }
                           , { 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, info.color,  s.str().c_str()}
                              ,{ft, id, render::FIND_OBJECT_TYPE::FOT_ROUTE_POINT, ui.value } });
}

void TrackLayerHelper::renderRouteSegment(render::iRender* renderer, colreg::id_type id, size_t index, const colreg::route_point& rp1, const colreg::route_point& rp2, const render::object_info& info, ColregSimulation::ROUTE_TYPE type)
{
   const auto ft = type == ColregSimulation::ROUTE_TYPE::RT_SUB_OPTIMAL ? render::FIND_TYPE::FT_NONE : render::FIND_TYPE::FT_FIND_FAST;

   const auto dir = math::direction(rp1.pos, rp2.pos);
   if (rp2.left_XTE < 100000)
   {
      const colreg::route_point rpLeft1 = math::calc_point(rp1.pos, rp2.left_XTE, dir - 90);
      const colreg::route_point rpLeft2 = math::calc_point(rp2.pos, rp2.left_XTE, dir - 90);
      renderer->AddObject({ { rpLeft1.pos, rpLeft2.pos }, {1, render::LINE_STYLE::LL_DOT, render::FILL_TYPE::FT_NONE, info.color, ""} });
   }
   if (rp2.right_XTE < 100000)
   {
      const colreg::route_point rpRight1 = math::calc_point(rp1.pos, rp2.right_XTE, dir + 90);
      const colreg::route_point rpRight2 = math::calc_point(rp2.pos, rp2.right_XTE, dir + 90);
      renderer->AddObject({ { rpRight1.pos, rpRight2.pos }, {1, render::LINE_STYLE::LL_DOT, render::FILL_TYPE::FT_NONE, info.color, "" } });
   }
   renderer->AddObject({ { rp1.pos, rp2.pos }
                           , info
                             , {ft, id, render::FIND_OBJECT_TYPE::FOT_ROUTE_SEGMENT, index } });
}


void TrackLayerHelper::renderRouteZone(render::iRender* renderer, const std::function<RouteZoneWidthGetFunc>& func, const colreg::route_ref* route, COLORREF color)
{
   using namespace colreg;
   if (!route || route->size < 2)
      return;


   //const ROUTE_ZONE_TYPE zones[] = { ROUTE_ZONE_TYPE::RZT_IN_ROUTE, ROUTE_ZONE_TYPE::RZT_CLOSE_TO_ROUTE, ROUTE_ZONE_TYPE::RZT_NEAR_TO_ROUTE };

   const int alphas[] = { 32, 32, 4 };
   const COLORREF colors[] = { RGB(0,255, 0), RGB(255,255, 0), RGB(255,0,0) };
   render::object_info oi;
   oi.fill = render::FILL_TYPE::FT_SOLID;
   oi.style = render::LINE_STYLE::LL_DOT;
   oi.width = 1;
   for (size_t i = 0; i < route->size - 1; i++)
   {
      const route_point& rp = route->arr[i];
      const route_point& rpNext = route->arr[i + 1];
      const auto dir = math::direction(rp.pos, rpNext.pos);
      //TODO: calc directions and save to and cos into

      const auto dirDeltaPrev = i ? math::calc_DK(dir, math::direction(route->arr[i - 1], route->arr[i])) / 2 : 0.0;
      const auto dirDeltaNext = ((i + 2) < route->size) ? math::calc_DK(dir, math::direction(route->arr[i + 1], route->arr[i + 2])) / 2 : 0.0;

      const auto cosPrev = cos(math::grad_to_rad(dirDeltaPrev));
      const auto cosNext = cos(math::grad_to_rad(dirDeltaNext));

      double leftZoneWidth = 0.0;
      double rightZoneWidth = 0.0;
      double leftZoneWidthN = 0.0;
      double rightZoneWidthN = 0.0;

      /*for (const auto zone : zones)
      {
         auto leftIdeal = func(i, true, zone);
         auto rightIdeal = func(i, false, zone);
         leftZoneWidth += leftIdeal / cosPrev;
         rightZoneWidth += rightIdeal / cosPrev;

         leftZoneWidthN += leftIdeal / cosNext;
         rightZoneWidthN += rightIdeal / cosNext;

         auto p1 = math::calc_point(rp.pos, leftZoneWidth, dir - 90 + dirDeltaPrev);
         auto p2 = math::calc_point(rpNext.pos, leftZoneWidthN, dir - 90 + dirDeltaNext);
         auto p3 = math::calc_point(rpNext.pos, rightZoneWidthN, dir + 90 + dirDeltaNext);
         auto p4 = math::calc_point(rp.pos, rightZoneWidth, dir + 90 + dirDeltaPrev);
         oi.color = colors[(size_t)zone];
         oi.alpha = alphas[(size_t)zone];
         renderer->AddObject({ { p1, p2, p3, p4 }, oi });

      }*/
   }
}