#include "stdafx.h"
#include "createobjecttool.h"
//#include "Colreg\simulator.h"
//#include "Selection\SelectetObjectManager.h"
#include "scenario/ScenarioManager.h"
//#include "Layers\renderhelper.h"

using namespace SV;
using namespace SV::user_interface;

void CreateObjectTool::MouseMove(CPoint point)
{
   if (!_renderer)
      return;

   //_pointLast = _renderer->PixelToGeo(SVCG::math_point{ (double)point.y, (double)point.x });
   //renderObject();
   InvalidateView();
   BaseTool::MouseMove(point);
}


void CreateObjectTool::LmouseDown(CPoint point)
{
   BaseTool::LmouseDown(point);
   //_points.push_back(_renderer->PixelToGeo(SVCG::math_point{ (double)point.y, (double)point.x }));

   //if (_createType == CREATE_TYPE::CT_CHART_OBJ && colreg::check_chart_obj_type(colreg::OT_POINTS, _type))
   //   addObject();

   //renderObject();
}

void CreateObjectTool::LmouseUp(CPoint point)
{
   InvalidateView();

   BaseTool::LmouseUp(point);
}

void CreateObjectTool::MmouseDown(CPoint point)
{

}

void CreateObjectTool::MmouseUp(CPoint point)
{

}

void CreateObjectTool::RmouseDown(CPoint point)
{
   //if (_points.empty())
   //{
   //   user_interface::SetEditMode(EDIT_MODE::EM_DRAG);
   //   return;
   //}
   //if (_createType == CREATE_TYPE::CT_SHIP)//ship
   //{
   //   _route = std::vector<colreg::route_point>(_points.size());

   //   for (size_t i = 0; i < _points.size(); ++i)
   //   {
   //      _route[i].pos = _points[i];
   //      _route[i].speed = _shipPos.speed;
   //      _route[i].radius = 1.;
   //      _route[i].left_XTE = _route[i].right_XTE = .5;
   //   }
   //   ScenarioManager::GetInstance().AddShip(_shipPos, _route);
   //   _points.clear();
   //}
   //else if (_createType == CREATE_TYPE::CT_SIMULATION_ROUTE //sim route
   //   || _createType == CREATE_TYPE::CT_SOURCE_ROUTE)
   //{
   //   _route = std::vector<colreg::route_point>(_points.size());
   //   const auto id = SelectetObjectManager::GetInstance().GetSelectedShip();
   //   const auto speed = id != colreg::INVALID_ID ? simulator::getShip(id)->GetPos().point.speed : 10.;
   //   for (size_t i = 0; i < _points.size(); ++i)
   //   {
   //      _route[i].pos = _points[i];
   //      _route[i].speed = speed;
   //      _route[i].radius = 1.;
   //      _route[i].left_XTE = _route[i].right_XTE = .5;
   //   }
   //   if (id != colreg::INVALID_ID)
   //      ScenarioManager::GetInstance().SetShipRoute(_createType == CREATE_TYPE::CT_SIMULATION_ROUTE ? surface_simulation::ROUTE_TYPE::RT_SIMULAION : surface_simulation::ROUTE_TYPE::RT_SOURSE
   //         , id, _route);
   //   _points.clear();
   //}
   //else if (!_points.empty())
   //   addObject();

   InvalidateView();
   //user_interface::SetEditMode(EDIT_MODE::EM_DRAG);
}

void CreateObjectTool::RmouseUp(CPoint point)
{
   //_points.clear();
}

void CreateObjectTool::MouseLeave()
{

}

//void CreateObjectTool::renderObject()
//{
//   if (_points.empty())
//      return;
//
//   auto points = _points;
//
//   bool area = _points.size() > 2 && _createType == CREATE_TYPE::CT_CHART_OBJ &&
//      ((colreg::check_chart_obj_type(colreg::OT_AREAS, _type) || colreg::check_chart_obj_type(colreg::OT_DYNAMIC_AREAS, _type) &&
//         !colreg::check_chart_obj_type(colreg::OT_XTE_AREA, _type)));
//
//   _renderer->AddObject({ points, {5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE , RGB(255, 200, 0)} });
//   _renderer->AddObject({ {_points.back(), _pointLast}, {1, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, RGB(255, 200, 0)} });
//
//
//   if (area)
//   {
//      _renderer->AddObject({ {_points.front(), _pointLast}, {1, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, RGB(255, 200, 0)} });
//      points.push_back(_pointLast);
//      _renderer->AddObject({ points, {2, render::LINE_STYLE::LL_NONE, render::FILL_TYPE::FT_SOLID, RGB(0, 0, 200), "", 0, 0, 50} });
//   }
//
//   for (const auto& p : _points)
//      _renderer->AddObject({ {p}, {5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(0, 0, 200), ""} });
//
//
//   std::stringstream text;
//   text << "dir: " << get_formated_value(math::direction(_points.back(), _pointLast), VALUE_FORMAT_TYPE::VFT_COURSE);
//   text << ", d: " << get_formated_value(math::distance(_points.back(), _pointLast), VALUE_FORMAT_TYPE::VFT_DISTANCE);
//
//   _renderer->AddObject({ {_pointLast}, {5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(200, 0, 0), text.str(), 25, 0} });
//
//   if (_type == colreg::OBJECT_TYPE::OT_NONE)
//      renderShip();
//}
//
//void CreateObjectTool::renderShip()
//{
//   if (_points.empty())
//      return;
//   colreg::settings s;
//
//   colreg::ship_info info;
//   info.width = (int)s.defaultShipWidth;
//   info.length = (int)s.defaultShipLength;
//   info.draft = (int)s.defaultShipDraft;
//
//   _shipPos.pos = _points[0];
//   _shipPos.heading = _shipPos.course = math::direction(_shipPos.pos, _pointLast);
//   _shipPos.speed = 10.;
//
//   RenderShipContour(_renderer, info, _shipPos, { 1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_SOLID, RGB(255, 200, 0) });
//}
//
//void CreateObjectTool::addObject()
//{
//   colreg::chart_object object;
//   object.type = _type;
//
//   if (colreg::check_chart_obj_type(colreg::OT_AREAS, _type) && !colreg::check_chart_obj_type(colreg::OT_XTE_AREA, _type))
//      _points.push_back(_points[0]);
//   std::vector<colreg::geo_point> coordinates(_points);
//   colreg::geo_points_ref ref = { coordinates.data(), coordinates.size() };
//   object.geom = colreg::object_geometry_ref{ &ref , 1 };
//   ScenarioManager::GetInstance().AddObject(object);
//   _points.clear();
//}