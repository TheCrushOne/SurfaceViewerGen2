#include "stdafx.h"
#include "gui/selection/SelectedObjectManager.h"
//#include "selectedship.h"
#include "SelectedRoute.h"
#include "SelectedChartObject.h"
#include "ScenarioProperties.h"
#include "gui/user_interface.h"

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
   //case render::FIND_OBJECT_TYPE::FOT_TRACK:
   case render::FIND_OBJECT_TYPE::FOT_ROVER:
   case render::FIND_OBJECT_TYPE::FOT_DOMAIN:
   {
      _selected = std::make_unique <SelectedRover>(info.id);
      _selected_ship_id = info.id;
      break;
   }
   case render::FIND_OBJECT_TYPE::FOT_ROUTE_POINT:
   {
      _selected = std::make_unique <SelectedRoutePoint>(info.id, info.user_data);
      break;
   }
   case render::FIND_OBJECT_TYPE::FOT_ROUTE_SEGMENT:
   {
      _selected = std::make_unique <SelectedRouteSegment>(info.id, info.user_data);
      break;
   }
   case render::FIND_OBJECT_TYPE::FOT_CHART_OBJECT:
   {
      _selected_chart_id = info.id;
      _selected = std::make_unique <SelectedChartObject>(info.id, info.chart_object_type);
      break;
   }
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
