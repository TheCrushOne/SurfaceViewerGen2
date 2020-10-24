#include "stdafx.h"
#include "mousetool.h"
#include "dragtool.h"
#include "rulertool.h"
#include "createobjecttool.h"
//#include "colreg/ChartSafetyStructs.h"
using namespace SV;
using namespace SV::user_interface;

iMouseToolPtr create_mouse_tool(EDIT_MODE type, render::iRender* renderer, unsigned long long int userData)
{
   switch (type)
   {
   case EDIT_MODE::EM_EDIT:
   case EDIT_MODE::EM_DRAG:      return std::make_unique<DragTool>(renderer);
   case EDIT_MODE::EM_MEASURMENT:return std::make_unique<RulerTool>(renderer);
   case EDIT_MODE::EM_CREATE:    return std::make_unique<CreateObjectTool>(renderer/*, static_cast<colreg::OBJECT_TYPE>(userData)*/);
   }
   ATLASSERT("NOT IMPLEMENTED!!!");
   return std::make_unique<DragTool>(renderer);
}

void BaseTool::MouseMove(CPoint point)
{
   if (!_renderer)
      return;

   const auto pix = _renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });
   std::wstringstream s;
   s.width(4);
   s << L"Lat: " << pix.lat << L", Lon: " << pix.lon;
   SetStatusBarText(s.str().c_str());
}

void BaseTool::MouseWheele(short zDelta, CPoint point)
{
   const auto geo = _renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });
   _renderer->SetScale(_renderer->GetScale() * (zDelta > 0 ? 1.1 : 0.9));

   const auto size = _renderer->GetSize();
   const auto dir = math::direction(math::point{ (double)size.cy * .5, (double)size.cx * .5 }, math::point{ (double)point.y, (double)point.x });
   const auto dist = math::distance(math::point{ (double)size.cy * .5, (double)size.cx * .5 }, math::point{ (double)point.y, (double)point.x });
   const auto pixMousePos = _renderer->GeoToPixel(geo);
   const auto pixCenterPos = math::calc_point(pixMousePos, dist, dir + 180);
   const auto geoCenterPos = _renderer->PixelToGeo(pixCenterPos);

   _renderer->SetCenter(geoCenterPos);
   InvalidateView();
}

