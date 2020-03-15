#include "stdafx.h"
#include "renderhelper.h"
#include "colreg/domain_utils.h"


void	RenderUnitContour(render::iRender* renderer, ColregSimulation::UNIT_TYPE type, const colreg::ship_info& ship_info, const colreg::track_point_info& center, const render::object_info& info)
{
   switch (type)
   {
   case ColregSimulation::UNIT_TYPE::UT_ROVER:
   {
      const auto heading = center.heading == colreg::NO_VALUE ? center.course : center.heading;
      const auto L = ship_info.length * METERS_TO_MILE;
      const auto W = ship_info.width * METERS_TO_MILE;
      const auto N = W * sqrt(2 * .5);

      auto p1 = math::calc_point(center.pos, L * .5, heading + 180.);
      p1 = math::calc_point(p1, W * .5, heading - 90.);
      auto p2 = math::calc_point(p1, L - W * .5, heading);
      auto p3 = math::calc_point(p2, N, heading + 45.);
      auto p4 = math::calc_point(p3, N, heading + 135.);
      auto p5 = math::calc_point(p4, L - W * .5, heading + 180.);

      renderer->AddObject({ { p1, p2, p3, p4, p5, p1 }
                           , info
                           ,{render::FIND_TYPE::FT_FIND_FAST, ship_info.id, render::FIND_OBJECT_TYPE::FOT_ROVER } });
      break;
   }
   case ColregSimulation::UNIT_TYPE::UT_SHIP:
   {
      //const auto heading = 0;
      //const auto L = 2 * METERS_TO_MILE;
      //const auto W = 1 * METERS_TO_MILE;

      //const auto N = W * sqrt(2 * .5);

      //auto p1 = math::calc_point(center.pos, L * .5, heading + 180.);
      //p1 = math::calc_point(p1, W * .5, heading - 90.);
      //auto p2 = math::calc_point(p1, W, heading + 90);
      //auto p3 = math::calc_point(p2, N, heading + 45.);
      //auto p4 = math::calc_point(p3, N, heading + 135.);
      //auto p5 = math::calc_point(p4, L - W * .5, heading + 180.);
      std::string imagePath = SVGUtils::CurrentCurrentPath() + "\\citroen.png";
      COLORREF clrDanger = 255;
      renderer->AddObject({ { center.pos/*ship->GetPos().point.pos*/  }
                           , { 32, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, clrDanger, "[string]", 0, 0, 255, imagePath.c_str()}
                           , {} });
      //renderer->AddObject({ { p1, p2, p3, p4, p5, p1 }
      //                     , info
      //                     ,{render::FIND_TYPE::FT_FIND_FAST, ship_info.id, render::FIND_OBJECT_TYPE::FOT_SHIP } });
      break;
   }
   default:
      break;
   }
}


void RenderDomain(render::iRender* renderer, const ColregSimulation::iUnit& unit, const colreg::track_point_info& center, double timeFromNow, const render::object_info& info, const colreg::domain_scales* scales)
{
   auto domainTopology = unit.GetDomainTopology(timeFromNow, scales);
   if (!domainTopology)
      return;

   const auto metersInPix = MILE / renderer->GetScale();
   constexpr auto precisionPix = 10;
   const auto precisionMeters = metersInPix * precisionPix;
   const auto domainContour = colreg::calculate_domain_contour(domainTopology, precisionMeters);

   math::geo_points points(domainContour.size());
   for (size_t i = 0; i < domainContour.size(); ++i)
      points[i] = math::calc_point(center.pos, domainContour[i].distance, domainContour[i].course + center.heading);
   renderer->AddObject({ points
                        , info
                        ,{render::FIND_TYPE::FT_FIND_FAST, unit.GetInfo().id, render::FIND_OBJECT_TYPE::FOT_DOMAIN } });

}

void RenderArrow(render::iRender* renderer, const math::geo_point& pTo, double direction, const render::object_info& info, double sizeKoef)
{
   const auto length = math::distance(renderer->PixelToGeo(math::point{}), renderer->PixelToGeo(math::point{ 10., 10. })) * sizeKoef;

   math::geo_point& pStart = math::calc_point(pTo, length * 3, direction + 180);
   math::geo_point& p1 = math::calc_point(pStart, length, direction + 90);
   math::geo_point& p2 = math::calc_point(pStart, length, direction - 90);

   renderer->AddObject({ {p1, pTo, p2}, info });
}