#include "stdafx.h"
#include "selectedroute.h"
#include "colreg/ColregSimulation.h"
#include "simulator/simulator.h"

inline const char* route_type_to_string(ColregSimulation::ROUTE_TYPE _type)
{
   switch (_type)
   {
   case ColregSimulation::ROUTE_TYPE::RT_COLREG:
      return "COLREG";
   case ColregSimulation::ROUTE_TYPE::RT_SIMULAION:
      return "SIMULATION";
   case ColregSimulation::ROUTE_TYPE::RT_SOURSE:
      return "SOURCE";
   case ColregSimulation::ROUTE_TYPE::RT_DISIGION:
      return "DISIGION";
   }
   return "";
}

SelectRouteBase::SelectRouteBase(colreg::id_type id, size_t data)
   : _id{ id }
{
   user_interface::user_info ui;
   ui.value = data;

   _index = ui.index;
   _format_type = (ColregSimulation::ROUTE_TYPE)ui.type;

   const auto* sim = simulator::getSimulator();
   if (!sim)
      return;
   const auto& simulationState = sim->GetState();
   auto& route = simulationState.GetUnitById(id)->GetRoute(_format_type)->route;
   _route = std::vector<colreg::route_point>{ route->begin(), route->end()};
//   _route = ScenarioManager::GetInstance().GetRoute(_format_type, id).route;

   _ship_info_folder = std::make_unique< FolderProperty>("Ship info");

   _prop_id = std::make_unique< ValuePropertyHolder< SelectRouteBase, decltype(_id)>>
      ("ID", "Ship ID", true, VALUE_FORMAT_TYPE::VFT_NONE, this, &SelectRouteBase::_id, &SelectRouteBase::OnEmpty, this);

   _ship_info_folder->AddChild(_prop_id.get());


   _route_info_folder = std::make_unique< FolderProperty>("Route info");

   _typeName = route_type_to_string(_format_type);

   _route_type = std::make_unique< ValuePropertyHolder< SelectRouteBase, decltype(_typeName)>>
      ("Type", "Route type", true, VALUE_FORMAT_TYPE::VFT_NONE, this, &SelectRouteBase::_typeName, &SelectRouteBase::OnEmpty, this);

   _route_info_folder->AddChild(_route_type.get());

   AddChild(_ship_info_folder.get());
   AddChild(_route_info_folder.get());

}

SelectedRoutePoint::SelectedRoutePoint(colreg::id_type id, size_t data)
   : SelectRouteBase{ id, data }
{
   if (_index < _route.size())
   {
      _point = _route[_index];

      _point_folder = std::make_unique<FolderProperty>("Route point info");

      _prop_index = std::make_unique< ValuePropertyHolder< SelectedRoutePoint, decltype(_index)>>
         ("Index", "Index of the route segment", true, VALUE_FORMAT_TYPE::VFT_NONE, this, &SelectedRoutePoint::_index, &SelectedRoutePoint::OnSimSettingChanged, this);

      _prop_radius = std::make_unique< ValuePropertyHolder< colreg::route_point, decltype(_point.radius)>>
         ("radius", "circulation radius", false, VALUE_FORMAT_TYPE::VFT_DISTANCE, &_point, &colreg::route_point::radius, &SelectedRoutePoint::OnSimSettingChanged, this);

      _point_folder->AddChild(_prop_index.get());
      _point_folder->AddChild(_prop_radius.get());

      AddChild(_point_folder.get());
   }
}

void SelectedRoutePoint::OnSimSettingChanged()
{
   //_route[_index] = _point;
   //ScenarioManager::GetInstance().SetShipRoute(_format_type, _id, _route);
}


void SelectedRoutePoint::Render(render::iRender* renderer)
{
   renderer->AddObject({ { _point.pos }, { 8, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha }
                        ,{render::FIND_TYPE::FT_FIND_FAST, 0, render::FIND_OBJECT_TYPE::FOT_SELECTED, 0 } });
}

void SelectedRoutePoint::StartEdit(render::iRender* renderer, CPoint point, render::find_info info)
{
   _geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });
}

void SelectedRoutePoint::Edit(render::iRender* renderer, CPoint point)
{
   _geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });

   if (_index > 0)
   {
      renderer->AddObject({ { _route[_index - 1].pos, _geoEdit }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      std::stringstream text;
      text << "dir: " << get_formated_value(math::direction(_route[_index - 1].pos, _geoEdit), VALUE_FORMAT_TYPE::VFT_COURSE);
      text << ", d: " << get_formated_value(math::distance(_route[_index - 1].pos, _geoEdit), VALUE_FORMAT_TYPE::VFT_DISTANCE);

      renderer->AddObject({ {_geoEdit }, { 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, text.str(), 25, 0, user_interface::selectedAlpha } });
   }

   if (_index < _route.size() - 1)
      renderer->AddObject({ { _geoEdit,  _route[_index + 1].pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });

}

void SelectedRoutePoint::EndEdit()
{
   //_route[_index].pos = _geoEdit;
   //ScenarioManager::GetInstance().SetShipRoute(_format_type, _id, _route);
   SelectedObjectManager::GetInstance().Unselect();
}


void SelectedRoutePoint::Delete()
{
   _route.erase(_route.begin() + _index);
   //ScenarioManager::GetInstance().SetShipRoute(_format_type, _id, _route);
   SelectedObjectManager::GetInstance().Unselect();
}



SelectedRouteSegment::SelectedRouteSegment(colreg::id_type id, size_t data)
   : SelectRouteBase{ id, data }
{
   if (_index >= _route.size())
      return;
   _pointFrom = _route[_index];
   _pointTo = _route[_index + 1];
   _direction = math::direction(_pointFrom, _pointTo);

   _distance = math::distance(_pointFrom, _pointTo);
   _xte_left = _route[_index + 1].left_XTE;
   _xte_right = _route[_index + 1].right_XTE;
   _segment_folder = std::make_unique< FolderProperty>("Route segment info");

   _prop_index = std::make_unique< ValuePropertyHolder< SelectedRouteSegment, decltype(_index)>>
      ("Index", "Index of the route segment", true, VALUE_FORMAT_TYPE::VFT_NONE, this, &SelectedRouteSegment::_index, &SelectedRouteSegment::OnSimSettingChanged, this);

   _prop_speed = std::make_unique< ValuePropertyHolder< colreg::route_point, decltype(_pointTo.speed)>>
      ("speed", "speed on segment", false, VALUE_FORMAT_TYPE::VFT_SPEED, &_pointTo, &colreg::route_point::speed, &SelectedRouteSegment::OnSimSettingChanged, this);

   _prop_direction = std::make_unique< ValuePropertyHolder<SelectedRouteSegment, decltype(_direction)>>
      ("direction", "segment direction", true, VALUE_FORMAT_TYPE::VFT_COURSE, this, &SelectedRouteSegment::_direction, &SelectedRouteSegment::OnSimSettingChanged, this);

   _prop_distance = std::make_unique< ValuePropertyHolder<SelectedRouteSegment, decltype(_distance)>>
      ("distance", "segment distance", true, VALUE_FORMAT_TYPE::VFT_DISTANCE, this, &SelectedRouteSegment::_distance, &SelectedRouteSegment::OnSimSettingChanged, this);

   _prop_xte_left = std::make_unique< ValuePropertyHolder<SelectedRouteSegment, decltype(_xte_left)>>
      ("xte left", "segment left XTE", false, VALUE_FORMAT_TYPE::VFT_DISTANCE, this, &SelectedRouteSegment::_xte_left, &SelectedRouteSegment::OnSimSettingChanged, this);

   _prop_xte_right = std::make_unique< ValuePropertyHolder<SelectedRouteSegment, decltype(_xte_right)>>
      ("xte right", "segment right XTE", false, VALUE_FORMAT_TYPE::VFT_DISTANCE, this, &SelectedRouteSegment::_xte_right, &SelectedRouteSegment::OnSimSettingChanged, this);

   _segment_folder->AddChild(_prop_index.get());
   _segment_folder->AddChild(_prop_direction.get());
   _segment_folder->AddChild(_prop_distance.get());
   _segment_folder->AddChild(_prop_speed.get());
   _segment_folder->AddChild(_prop_xte_left.get());
   _segment_folder->AddChild(_prop_xte_right.get());
   AddChild(_segment_folder.get());
}

void SelectedRouteSegment::OnSimSettingChanged()
{
   //_route[_index + 1].left_XTE = _xte_left;
   //_route[_index + 1].right_XTE = _xte_right;
   //_route[_index + 1].speed = _pointTo.speed;
   //ScenarioManager::GetInstance().SetShipRoute(_format_type, _id, _route);
}


void SelectedRouteSegment::Render(render::iRender* renderer)
{
   renderer->AddObject({ { _pointFrom.pos, _pointTo.pos }, { 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha }
                        ,{render::FIND_TYPE::FT_FIND_FAST, 0, render::FIND_OBJECT_TYPE::FOT_SELECTED, SP_SEGMENT } });

   renderer->AddObject({ {_pointFrom.pos}, {8, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NULL, user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha }
                        ,{render::FIND_TYPE::FT_FIND_FAST, 0, render::FIND_OBJECT_TYPE::FOT_SELECTED, SP_START_POINT } });
   renderer->AddObject({ {_pointTo.pos}, {8, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NULL, user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha}
                        ,{render::FIND_TYPE::FT_FIND_FAST, 0, render::FIND_OBJECT_TYPE::FOT_SELECTED, SP_END_POINT } });
}


void SelectedRouteSegment::StartEdit(render::iRender* renderer, CPoint point, render::find_info info)
{
   _editPart = (SEGMENT_PART)info.user_data;
   _geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });
}

void SelectedRouteSegment::Edit(render::iRender* renderer, CPoint point)
{
   _geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });
   std::stringstream text;
   switch (_editPart)
   {
   case SelectedRouteSegment::SP_START_POINT:
      if (_index > 0)
      {
         renderer->AddObject({ { _route[_index - 1].pos, _geoEdit }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
         renderer->AddObject({ { _geoEdit,  _route[_index + 1].pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE ,user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });

         text << "Dir: " << get_formated_value(math::direction(_route[_index - 1].pos, _geoEdit), VALUE_FORMAT_TYPE::VFT_COURSE);
         text << "\nD  : " << get_formated_value(math::distance(_route[_index - 1].pos, _geoEdit), VALUE_FORMAT_TYPE::VFT_DISTANCE);
      }
      else
         renderer->AddObject({ { _geoEdit,  _route[_index + 1].pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      break;
   case SelectedRouteSegment::SP_END_POINT:
      if (_index < _route.size() - 1)
      {
         renderer->AddObject({ { _route[_index].pos, _geoEdit }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
         renderer->AddObject({ { _geoEdit,  _route[_index + 2].pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      }
      else
         renderer->AddObject({ { _geoEdit,  _route[_index + 1].pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      break;
   case SelectedRouteSegment::SP_SEGMENT:

      renderer->AddObject({ { _pointFrom.pos, _geoEdit }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      renderer->AddObject({ { _geoEdit,  _pointTo.pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      break;
   }


   renderer->AddObject({ { _geoEdit }, { 8, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, text.str(), 25, 0, user_interface::selectedAlpha } });
}

void SelectedRouteSegment::EndEdit()
{
   //switch (_editPart)
   //{
   //case SelectedRouteSegment::SP_START_POINT:
   //   _route[_index] = _geoEdit;
   //   break;
   //case SelectedRouteSegment::SP_END_POINT:
   //   _route[_index + 1] = _geoEdit;
   //   break;
   //case SelectedRouteSegment::SP_SEGMENT:
   //   _route.insert(_route.begin() + _index + 1, _geoEdit);
   //   _route[_index + 1] = _route[_index];
   //   _route[_index + 1].pos = _geoEdit;
   //   break;
   //}
   //ScenarioManager::GetInstance().SetShipRoute(_format_type, _id, _route);
   SelectedObjectManager::GetInstance().Unselect();
}


void SelectedRouteSegment::Delete()
{
   //_route.erase(_route.begin() + _index + 1);
   //_route.erase(_route.begin() + _index);
   //ScenarioManager::GetInstance().SetShipRoute(_format_type, _id, _route);
   SelectedObjectManager::GetInstance().Unselect();
}