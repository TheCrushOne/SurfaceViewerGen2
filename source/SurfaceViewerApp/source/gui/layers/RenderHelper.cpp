#include "stdafx.h"
#include "RenderHelper.h"
#include "colreg/domain_utils.h"
#include "math/math_utils.h"

using namespace SV;

void SV::RenderUnitContour(render::iRender* renderer, surface_simulation::UNIT_TYPE type, const layer_provider::ship_info& ship_info, const CG::layer_provider::trajectory_point& center, const render::object_info& info)
{
   switch (type)
   {
   case surface_simulation::UNIT_TYPE::UT_SHIP:
   {
      const auto heading = center.heading;
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
                           , {render::FIND_TYPE::FT_FIND_FAST, ship_info.id, render::FIND_OBJECT_TYPE::FOT_ROVER } });
      break;
   }
   case surface_simulation::UNIT_TYPE::UT_ROVER:
   {
      const auto heading = center.heading;
      std::string imagePath = SVGUtils::CurrentCurrentPath() + "\\res\\glyphicon\\citroen.png";
      COLORREF clrDanger = 255;
      renderer->AddObject({ { center.pos }
                           , { 32, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, clrDanger, /*ship_info.name*/"", 0, 0, 255, imagePath.c_str()}
                           , {}
                           , .0
                           , heading });
      break;
   }
   case surface_simulation::UNIT_TYPE::UT_DRONE:
   {
      const auto heading = center.heading;
      std::string imagePath = SVGUtils::CurrentCurrentPath() + "\\res\\glyphicon\\helicopter.png";
      COLORREF clrDanger = 255;
      renderer->AddObject({ { center.pos }
                           , { 32, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, clrDanger, /*ship_info.name*/"", 0, 0, 255, imagePath.c_str()}
                           , {}
                           , .0
                           , heading });
      break;
   }
   default:
      break;
   }
}


void SV::RenderDomain(render::FIND_OBJECT_TYPE fot, render::iRender* renderer, const surface_simulation::iLayerUnit* unit, const CG::layer_provider::trajectory_point& center, double timeFromNow, const render::object_info& info/*, const colreg::domain_scales* scales*/)
{
   // TODO: включить при необходимости
   /*auto domainTopology = unit->GetDomainTopology(timeFromNow, scales);
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
                        , { render::FIND_TYPE::FT_FIND_FAST, unit->GetInfo().id, fot } });*/
}

void SV::RenderArrow(render::iRender* renderer, const CG::geo_point& pTo, double direction, const render::object_info& info, double sizeKoef)
{
   const auto length = math::distance(renderer->PixelToGeo(math::point{}), renderer->PixelToGeo(math::point{ 5., 5. })) * sizeKoef;

   CG::geo_point& pStart = math::calc_point(pTo, length * 3, direction + 180);
   CG::geo_point& p1 = math::calc_point(pStart, length, direction + 90);
   CG::geo_point& p2 = math::calc_point(pStart, length, direction - 90);

   renderer->AddObject({ {p1, pTo, p2}, info });
}