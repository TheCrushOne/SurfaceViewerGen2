#include "stdafx.h"
#include "selectedroute.h"
#include "simulator/simulator.h"

using namespace SV;

inline const char* route_type_to_string(surface_simulation::ROUTE_TYPE _type)
{
   switch (_type)
   {
   case surface_simulation::ROUTE_TYPE::RT_COLREG:
      return "COLREG";
   case surface_simulation::ROUTE_TYPE::RT_SIMULAION:
      return "SIMULATION";
   case surface_simulation::ROUTE_TYPE::RT_SOURSE:
      return "SOURCE";
   case surface_simulation::ROUTE_TYPE::RT_DISIGION:
      return "DISIGION";
   }
   return "";
}

SelectRouteBase::SelectRouteBase(id_type id, size_t data)
   : m_id{ id }
{
   user_interface::user_info ui;
   ui.value = data;

   m_index = ui.index;
   m_format_type = (surface_simulation::ROUTE_TYPE)ui.type;

   const auto* sim = simulator::getSimulator();
   if (!sim)
      return;
   const auto& simulationState = sim->GetState();
   m_route = simulationState.GetUnitById(id)->GetRoute(m_format_type);
   
//   _route = ScenarioManager::GetInstance().GetRoute(_format_type, id).route;

   m_ship_info_folder = std::make_unique<FolderProperty>("Ship info");

   m_prop_id = std::make_unique< ValuePropertyHolder< SelectRouteBase, decltype(m_id)>>(
      FieldMeta{ "ID", "Ship ID", VALUE_FORMAT_TYPE::VFT_NONE, true },
      this, &SelectRouteBase::m_id, &SelectRouteBase::OnEmpty, this
   );

   m_ship_info_folder->AddChild(m_prop_id.get());


   m_route_info_folder = std::make_unique<FolderProperty>("Route info");

   m_typeName = route_type_to_string(m_format_type);

   m_route_type = std::make_unique< ValuePropertyHolder< SelectRouteBase, decltype(m_typeName)>>(
      FieldMeta{ "Type", "Route type", VALUE_FORMAT_TYPE::VFT_NONE, true },
      this, &SelectRouteBase::m_typeName, &SelectRouteBase::OnEmpty, this
   );

   m_route_info_folder->AddChild(m_route_type.get());

   AddChild(m_ship_info_folder.get());
   AddChild(m_route_info_folder.get());
}

SelectedRoutePoint::SelectedRoutePoint(id_type id, size_t data)
   : SelectRouteBase{ id, data }
{
   if (m_index < m_route.size())
   {
      m_point = m_route[m_index];

      m_point_folder = std::make_unique<FolderProperty>("Route point info");

      m_prop_index = std::make_unique< ValuePropertyHolder< SelectedRoutePoint, decltype(m_index)>>(
         FieldMeta{ "Index", "Index of the route segment", VALUE_FORMAT_TYPE::VFT_NONE, true },
         this, &SelectedRoutePoint::m_index, &SelectedRoutePoint::OnSimSettingChanged, this
      );

      /*_prop_radius = std::make_unique<ValuePropertyHolder<CG::route_point, decltype(_point.radius)>>
         ("radius", "circulation radius", false, VALUE_FORMAT_TYPE::VFT_DISTANCE, &_point, &colreg::route_point::radius, &SelectedRoutePoint::OnSimSettingChanged, this);*/

      m_point_folder->AddChild(m_prop_index.get());
      //_point_folder->AddChild(_prop_radius.get());

      AddChild(m_point_folder.get());
   }
}

void SelectedRoutePoint::OnSimSettingChanged()
{
   //_route[_index] = _point;
   //ScenarioManager::GetInstance().SetShipRoute(_format_type, _id, _route);
}


void SelectedRoutePoint::Render(render::iRender* renderer)
{
   renderer->AddObject({
      { m_point },
      { 8, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha },
      {render::FIND_TYPE::FT_FIND_FAST, 0, render::FIND_OBJECT_TYPE::FOT_SELECTED, 0 }
   });
}

void SelectedRoutePoint::StartEdit(render::iRender* renderer, CPoint point, render::find_info info)
{
   m_geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });
}

void SelectedRoutePoint::Edit(render::iRender* renderer, CPoint point)
{
   m_geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });

   if (m_index > 0)
   {
      renderer->AddObject({ { m_route[m_index - 1].pos, m_geoEdit }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      std::stringstream text;
      text << "dir: " << get_formated_value(math::direction(m_route[m_index - 1].pos, m_geoEdit), VALUE_FORMAT_TYPE::VFT_COURSE);
      text << ", d: " << get_formated_value(math::distance(m_route[m_index - 1].pos, m_geoEdit), VALUE_FORMAT_TYPE::VFT_DISTANCE);

      renderer->AddObject({ { m_geoEdit }, { 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, text.str(), 25, 0, user_interface::selectedAlpha } });
   }

   if (m_index < m_route.size() - 1)
      renderer->AddObject({ { m_geoEdit, m_route[m_index + 1].pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
}

void SelectedRoutePoint::EndEdit()
{
   //_route[_index].pos = _geoEdit;
   //ScenarioManager::GetInstance().SetShipRoute(_format_type, _id, _route);
   SelectedObjectManager::GetInstance().Unselect();
}


void SelectedRoutePoint::Delete()
{
   m_route.erase(m_route.begin() + m_index);
   //ScenarioManager::GetInstance().SetShipRoute(_format_type, _id, _route);
   SelectedObjectManager::GetInstance().Unselect();
}

SelectedRouteSegment::SelectedRouteSegment(id_type id, size_t data)
   : SelectRouteBase{ id, data }
{
   if (m_index >= m_route.size())
      return;
   m_pointFrom = m_route[m_index];
   m_pointTo = m_route[m_index + 1];
   m_direction = math::direction(m_pointFrom, m_pointTo);

   m_distance = math::distance(m_pointFrom, m_pointTo);
   m_xte_left = m_route[m_index + 1].left_XTE;
   m_xte_right = m_route[m_index + 1].right_XTE;
   m_segment_folder = std::make_unique<FolderProperty>("Route segment info");

   m_prop_index = std::make_unique<ValuePropertyHolder<SelectedRouteSegment, decltype(m_index)>>(
      FieldMeta{ "Index", "Index of the route segment", VALUE_FORMAT_TYPE::VFT_NONE, true },
      this, &SelectedRouteSegment::m_index, &SelectedRouteSegment::OnSimSettingChanged, this
   );

   m_prop_speed = std::make_unique<ValuePropertyHolder<CG::layer_provider::trajectory_point, decltype(m_pointTo.speed)>>(
      FieldMeta{ "speed", "speed on segment", VALUE_FORMAT_TYPE::VFT_SPEED, false }, 
      &m_pointTo, &CG::layer_provider::trajectory_point::speed, &SelectedRouteSegment::OnSimSettingChanged, this
   );

   m_prop_direction = std::make_unique<ValuePropertyHolder<SelectedRouteSegment, decltype(m_direction)>>(
      FieldMeta{ "direction", "segment direction", VALUE_FORMAT_TYPE::VFT_COURSE, true },
      this, &SelectedRouteSegment::m_direction, &SelectedRouteSegment::OnSimSettingChanged, this
   );

   m_prop_distance = std::make_unique< ValuePropertyHolder<SelectedRouteSegment, decltype(m_distance)>>(
      FieldMeta{ "distance", "segment distance", VALUE_FORMAT_TYPE::VFT_DISTANCE, true },
      this, &SelectedRouteSegment::m_distance, &SelectedRouteSegment::OnSimSettingChanged, this
   );

   m_prop_xte_left = std::make_unique< ValuePropertyHolder<SelectedRouteSegment, decltype(m_xte_left)>>(
      FieldMeta{ "xte left", "segment left XTE", VALUE_FORMAT_TYPE::VFT_DISTANCE, false },
      this, &SelectedRouteSegment::m_xte_left, &SelectedRouteSegment::OnSimSettingChanged, this
   );

   m_prop_xte_right = std::make_unique< ValuePropertyHolder<SelectedRouteSegment, decltype(m_xte_right)>>(
      FieldMeta{ "xte right", "segment right XTE", VALUE_FORMAT_TYPE::VFT_DISTANCE, false },
      this, &SelectedRouteSegment::m_xte_right, &SelectedRouteSegment::OnSimSettingChanged, this
   );

   m_segment_folder->AddChild(m_prop_index.get());
   m_segment_folder->AddChild(m_prop_direction.get());
   m_segment_folder->AddChild(m_prop_distance.get());
   m_segment_folder->AddChild(m_prop_speed.get());
   m_segment_folder->AddChild(m_prop_xte_left.get());
   m_segment_folder->AddChild(m_prop_xte_right.get());
   AddChild(m_segment_folder.get());
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
   renderer->AddObject({ 
      { m_pointFrom.pos, m_pointTo.pos },
      { 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha },
      { render::FIND_TYPE::FT_FIND_FAST, 0, render::FIND_OBJECT_TYPE::FOT_SELECTED, SP_SEGMENT }
   });

   renderer->AddObject({
      { m_pointFrom.pos },
      { 8, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NULL, user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha },
      { render::FIND_TYPE::FT_FIND_FAST, 0, render::FIND_OBJECT_TYPE::FOT_SELECTED, SP_START_POINT }
   });

   renderer->AddObject({
      { m_pointTo.pos },
      { 8, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NULL, user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha},
      { render::FIND_TYPE::FT_FIND_FAST, 0, render::FIND_OBJECT_TYPE::FOT_SELECTED, SP_END_POINT }
   });
}


void SelectedRouteSegment::StartEdit(render::iRender* renderer, CPoint point, render::find_info info)
{
   m_editPart = (SEGMENT_PART)info.user_data;
   m_geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });
}

void SelectedRouteSegment::Edit(render::iRender* renderer, CPoint point)
{
   m_geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });
   std::stringstream text;
   switch (m_editPart)
   {
   case SelectedRouteSegment::SP_START_POINT:
      if (m_index > 0)
      {
         renderer->AddObject({
            { m_route[m_index - 1].pos, m_geoEdit },
            { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha }
         });
         renderer->AddObject({ { m_geoEdit,  m_route[m_index + 1].pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE ,user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });

         text << "Dir: " << get_formated_value(math::direction(m_route[m_index - 1].pos, m_geoEdit), VALUE_FORMAT_TYPE::VFT_COURSE);
         text << "\nD  : " << get_formated_value(math::distance(m_route[m_index - 1].pos, m_geoEdit), VALUE_FORMAT_TYPE::VFT_DISTANCE);
      }
      else
         renderer->AddObject({ { m_geoEdit, m_route[m_index + 1].pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      break;
   case SelectedRouteSegment::SP_END_POINT:
      if (m_index < m_route.size() - 1)
      {
         renderer->AddObject({ { m_route[m_index].pos, m_geoEdit }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
         renderer->AddObject({ { m_geoEdit, m_route[m_index + 2].pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      }
      else
         renderer->AddObject({ { m_geoEdit, m_route[m_index + 1].pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      break;
   case SelectedRouteSegment::SP_SEGMENT:

      renderer->AddObject({ { m_pointFrom.pos, m_geoEdit }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      renderer->AddObject({ { m_geoEdit, m_pointTo.pos }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      break;
   }


   renderer->AddObject({ { m_geoEdit }, { 8, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, text.str(), 25, 0, user_interface::selectedAlpha } });
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