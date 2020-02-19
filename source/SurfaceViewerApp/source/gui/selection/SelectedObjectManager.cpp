#include "stdafx.h"
#include "gui/selection/SelectedObjectManager.h"
//#include "selectedship.h"
#include "selectedroute.h"
#include "SelectedChartObject.h"
#include "ScenarioProperties.h"

std::vector< SelectObjectObserver* >AutoContainer< SelectObjectObserver>::_objects;

SelectedObjectManager::SelectedObjectManager()
{

}

bool SelectedObjectManager::Select(const render::find_info& info)
{
   _info = info;
   _selected_ship_id = colreg::INVALID_ID;
   _selected_chart_id = {};
   switch (info.find_object_type)
   {
   //case render::FIND_OBJECT_TYPE::FOT_SHIP:
   //case render::FIND_OBJECT_TYPE::FOT_DOMAIN:
   //{
   //   _selected = std::make_unique <SelectedShip>(info.id);
   //   _selected_ship_id = info.id;
   //   break;
   //}
   //case render::FIND_OBJECT_TYPE::FOT_ROUTE_POINT:
   //{
   //   _selected = std::make_unique <SelectedRoutePoint>(info.id, info.user_data);
   //   break;
   //}
   //case render::FIND_OBJECT_TYPE::FOT_ROUTE_SEGMENT:
   //{
   //   _selected = std::make_unique <SelectedRouteSegment>(info.id, info.user_data);
   //   break;
   //}
   //case render::FIND_OBJECT_TYPE::FOT_CHART_OBJECT:
   //{
   //   if (check_chart_obj_type(colreg::OBJECT_TYPE::OT_DYNAMIC_AREAS, info.chart_object_type))
   //   {
   //      _selected_chart_id.dynamic_id = info.id;
   //   }
   //   else
   //   {
   //      _selected_chart_id.static_id = info.id;
   //   }

   //   _selected = std::make_unique <SelectedChartObject>(info.id, info.chart_object_type);
   //   break;
   //}
   default:
      _selected = std::make_unique <ScenarioProperties>();
   }

   SelectObjectObserver::SelectObject(_selected.get());
   return true;
}

void SelectedObjectManager::Update()
{
   if (_selected && _selected->IsUpdateable())
      Select(_info);
}

void SelectedObjectManager::Unselect()
{
   _selected.reset();
   _info = {};
   SelectObjectObserver::SelectObject(nullptr);
   user_interface::InvalidateView();
}

void SelectedObjectManager::Delete()
{
   if (_selected)
      _selected->Delete();
   Unselect();
   SelectObjectObserver::SelectObject(nullptr);
}
