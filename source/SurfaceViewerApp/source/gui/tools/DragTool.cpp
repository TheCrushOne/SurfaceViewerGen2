#include "stdafx.h"
#include "dragtool.h"
//#include "Colreg\simulator.h"
#include "gui/selection\SelectedObjectManager.h"

using namespace user_interface;

void DragTool::MouseMove(CPoint point)
{
   if (!_renderer)
      return;
   if (_selected)
   {
      _selected->Edit(_renderer, point);
      BaseTool::MouseMove(point);
      InvalidateView();
      return;
   }
   if (_lbDown)
   {
      const auto dir = math::direction(math::point{ (double)_clickPos.y, (double)_clickPos.x }, math::point{ (double)point.y, (double)point.x });
      const auto dist = math::distance(math::point{ (double)_clickPos.y, (double)_clickPos.x }, math::point{ (double)point.y, (double)point.x }) / _renderer->GetScale();
      const auto newCenter = math::calc_point(_renderer->GetCenter(), dist, -dir);
      _renderer->SetCenter(newCenter);
      _clickPos = point;
      _moved = true;
      InvalidateView();
   }
   if (!processFindInfo(_renderer->FindObject(math::point{ (double)point.y, (double)point.x }, render::FIND_TYPE::FT_FIND_FAST)))
      HideToolTip();

   BaseTool::MouseMove(point);
}


void DragTool::LmouseDown(CPoint point)
{
   const auto find = _renderer->FindObject(math::point{ (double)point.y, (double)point.x }, render::FIND_TYPE::FT_FIND_DETAILED);

   if (GetEditMode() == EDIT_MODE::EM_EDIT && find.find_object_type == render::FIND_OBJECT_TYPE::FOT_SELECTED)
   {
      _selected = SelectedObjectManager::GetInstance().GetSelected();
      _selected->StartEdit(_renderer, point, find);
      return;
   }

   BaseTool::LmouseDown(point);
}

void DragTool::LmouseUp(CPoint point)
{
   if (_moved)
   {
      _moved = false;
   }
   else
   {
      if (_selected)
      {
         _selected->EndEdit();
         _selected = nullptr;
         InvalidateView();
         return;
      }

      const auto fo = _renderer->FindObject(math::point{ (double)point.y, (double)point.x }, render::FIND_TYPE::FT_FIND_DETAILED);
      switch (fo.find_object_type)
      {
         // FOG тут был
      default:
         SelectedObjectManager::GetInstance().Select(fo);
      }
   }
   InvalidateView();

   BaseTool::LmouseUp(point);
}

void DragTool::MmouseDown(CPoint point)
{

}

void DragTool::MmouseUp(CPoint point)
{

}

void DragTool::RmouseDown(CPoint point)
{

}

void DragTool::RmouseUp(CPoint point)
{
   _selected = nullptr;
   InvalidateView();
}

void DragTool::MouseLeave()
{

}


inline const char* route_type_to_string(ColregSimulation::ROUTE_TYPE _type) { return ""; }

bool DragTool::processFindInfo(const render::find_info& find)
{
   if (find.find_object_type == render::FIND_OBJECT_TYPE::FOT_NONE)
      return false;

   using namespace render;
   std::wstringstream title;
   std::wstringstream description;

   switch (find.find_object_type)
   {
   case FIND_OBJECT_TYPE::FOT_SELECTED:
      ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
      break;
   case FIND_OBJECT_TYPE::FOT_ROVER:
   case FIND_OBJECT_TYPE::FOT_DRONE:
   {
      //title << "Ship : " << find.id;
      //const auto* ship = simulator::getShip(find.id);
      //ATLASSERT(ship);
      //description << "AIS:    " << ais_status_to_string(ship->GetInfo().AIS_status) << "\n";
      //description << "draft:  " << get_formated_value(ship->GetInfo().draft, VALUE_FORMAT_TYPE::VFT_DISTANCE_METERS) << "\n";
      //description << "Lat:    " << get_formated_value(ship->GetPos().point.pos.lat, VALUE_FORMAT_TYPE::VFT_COORD_LAT).c_str() << "\n";
      //description << "Lon:    " << get_formated_value(ship->GetPos().point.pos.lon, VALUE_FORMAT_TYPE::VFT_COORD_LON).c_str() << "\n";
      //description << "Speed:  " << get_formated_value(ship->GetPos().point.speed, VALUE_FORMAT_TYPE::VFT_SPEED).c_str() << "\n";
      //description << "Course: " << get_formated_value(ship->GetPos().point.course, VALUE_FORMAT_TYPE::VFT_COURSE).c_str() << "\n";
      break;
   }
   //case FIND_OBJECT_TYPE::FOT_CHART_OBJECT:
   //{
   //   title << "Chart object id: " << find.id;
   //   colreg::OBJECT_TYPE type = (colreg::OBJECT_TYPE)find.user_data;
   //   description << colreg::chart_obj_type_to_str(type);
   //   break;
   //}
   case FIND_OBJECT_TYPE::FOT_ROUTE_POINT:
   {
      title << "Ship : " << find.id;
      user_info ui;
      ui.value = find.user_data;
      description << "Type: " << route_type_to_string((ColregSimulation::ROUTE_TYPE)ui.type) << "\n";
      description << "Route point: " << ui.index << "\n";

      break;
   }
   case FIND_OBJECT_TYPE::FOT_ROUTE_SEGMENT:
   {
      title << "Ship  " << find.id;
      user_info ui;
      ui.value = find.user_data;
      description << "Type          : " << route_type_to_string((ColregSimulation::ROUTE_TYPE)ui.type) << "\n";
      description << "segment       : " << ui.index << "\n";

      //const auto route_info = ScenarioManager::GetInstance().GetRoute((ColregSimulation::ROUTE_TYPE)ui.type, find.id);
      //ATLASSERT(route_info.route.size() > (size_t)(ui.index + 1));

      //description << "speed         : " << get_formated_value(route_info.route[ui.index + 1].speed, VALUE_FORMAT_TYPE::VFT_SPEED).c_str() << "\n";
      //description << "dir           : " << get_formated_value(math::direction(route_info.route[ui.index].pos, route_info.route[ui.index + 1].pos), VALUE_FORMAT_TYPE::VFT_COURSE) << "\n";
      //description << "dist          : " << get_formated_value(math::distance(route_info.route[ui.index].pos, route_info.route[ui.index + 1].pos), VALUE_FORMAT_TYPE::VFT_DISTANCE) << "\n";
      //if (route_info.maneuver_type_valid)
      {
         //description << "maneuver type : " << colreg::maneuver_to_string(route_info.maneuver_type) << "\n";
      }

      break;
   }

   }

   ShowToolTip(title.str().c_str(), description.str().c_str());
   return true;
}
