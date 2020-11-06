#include "stdafx.h"
#include "gui/selection/SelectedObjectManager.h"
#include "SelectedUnit.h"
#include "SelectedRoute.h"
#include "SelectedChartObject.h"
#include "ScenarioProperties.h"
#include "gui/user_interface.h"

using namespace SV;

std::vector< SelectObjectObserver* >AutoContainer< SelectObjectObserver>::_objects;

SelectedObjectManager::SelectedObjectManager()
{

}

bool SelectedObjectManager::Select(const render::find_info& info)
{
   m_info = info;
   m_selected_ship_id = INVALID_ID;
   m_selected_chart_id = {};
   switch (info.find_object_type)
   {
      case render::FIND_OBJECT_TYPE::FOT_SHIP:
      case render::FIND_OBJECT_TYPE::FOT_SHIP_DOMAIN:
      {
         m_selected = std::make_unique<SelectedShip>(info.id);
         m_selected_ship_id = info.id;
         break;
      }
      case render::FIND_OBJECT_TYPE::FOT_DRONE:
      case render::FIND_OBJECT_TYPE::FOT_DRONE_DOMAIN:
      {
         m_selected = std::make_unique<SelectedDrone>(info.id);
         m_selected_ship_id = info.id;
         break;
      }
      case render::FIND_OBJECT_TYPE::FOT_ROVER:
      case render::FIND_OBJECT_TYPE::FOT_ROVER_DOMAIN:
      {
         m_selected = std::make_unique<SelectedRover>(info.id);
         m_selected_ship_id = info.id;
         break;
      }
      case render::FIND_OBJECT_TYPE::FOT_ROUTE_POINT:
      {
         m_selected = std::make_unique<SelectedRoutePoint>(info.id, info.user_data);
         break;
      }
      case render::FIND_OBJECT_TYPE::FOT_ROUTE_SEGMENT:
      {
         m_selected = std::make_unique<SelectedRouteSegment>(info.id, info.user_data);
         break;
      }
      case render::FIND_OBJECT_TYPE::FOT_CHART_OBJECT:
      {
         m_selected_chart_id = info.id;
         m_selected = std::make_unique<SelectedChartObject>(info.id, info.chart_object_type);
         break;
      }
      default:
         m_selected = std::make_unique<ScenarioProperties>();
   }

   SelectObjectObserver::SelectObject(m_selected.get());
   return true;
}

void SelectedObjectManager::Update()
{
   if (m_selected && m_selected->IsUpdateable())
      Select(m_info);
}

void SelectedObjectManager::Unselect()
{
   m_selected.reset();
   m_info = {};
   SelectObjectObserver::SelectObject(nullptr);
   user_interface::InvalidateView();
}

void SelectedObjectManager::Delete()
{
   if (m_selected)
      m_selected->Delete();
   Unselect();
   SelectObjectObserver::SelectObject(nullptr);
}