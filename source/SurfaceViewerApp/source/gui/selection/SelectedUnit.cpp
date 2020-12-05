#include "stdafx.h"
#include "SelectedUnit.h"
#include "simulator/simulator.h"
#include "properties/property_helper.h"
#include "gui/layers/RenderHelper.h"
#include "gui/layers/TrackLayerHelper.h"
#include "simulation/LayerUnitInterface.h"
#include "simulation/LayerChartObjectInterface.h"
#include "gui\helpers\LayerFiltersManager.h"

#define CRSHIPINFO(iPropPtr, prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(iPropPtr, prStruct, obj, field, SelectedShip::shipInfoMeta, sifi, &SelectedShip::OnShipInfoChanged)

using namespace SV;

static const render::object_info selected_drone_domain_info
{
   3,
   render::LINE_STYLE::LL_SOLID,
   render::FILL_TYPE::FT_NULL,
   user_interface::selectedColor,
   "",
   0,
   0,
   user_interface::selectedAlpha
};

static const render::object_info selected_rover_domain_info
{
   3,
   render::LINE_STYLE::LL_SOLID,
   render::FILL_TYPE::FT_NULL,
   user_interface::selectedColor,
   "",
   0,
   0,
   user_interface::selectedAlpha
};

static const render::object_info selected_ship_domain_info
{
   3,
   render::LINE_STYLE::LL_SOLID,
   render::FILL_TYPE::FT_NULL,
   user_interface::selectedColor,
   "",
   0,
   0,
   user_interface::selectedAlpha
};

static const render::object_info selected_drone_track_info
{
   1,
   render::LINE_STYLE::LL_DASH,
   render::FILL_TYPE::FT_NONE,
   RGB(110, 20, 20)
};

static const render::object_info selected_rover_track_info
{
   1,
   render::LINE_STYLE::LL_DASH,
   render::FILL_TYPE::FT_NONE,
   RGB(110, 20, 20)
};

static const render::object_info selected_ship_track_info
{
   1,
   render::LINE_STYLE::LL_DASH,
   render::FILL_TYPE::FT_NONE,
   RGB(110, 20, 20)
};

SelectedUnit::SelectedUnit(id_type id, surface_simulation::UNIT_TYPE type)
   : m_id{ id }
{
   const auto* sim = simulator::getSimulator();
   if (!sim)
      return;
   const auto& simulationState = sim->GetState();
   auto* unit = simulationState.GetUnitById(id);
   // TODO: сделать проверку на возврат пустого юнита
   if (true)
   {
      mountUnitInfo(unit);
      mountTrackPointInfo(unit);
      mountSimSettings(unit);
      mountColregSettings(unit);
      mountSolution(unit);
      mountModel(unit);
   }
}

const surface_simulation::iSimulationState& SelectedUnit::getState() const
{
   const auto* sim = simulator::getSimulator();
   return sim->GetState();
}

void SelectedUnit::mountUnitInfo(const surface_simulation::iLayerUnit* selected)
{
   m_info = selected->GetInfo();
   m_ship_info_folder = std::make_unique<FolderProperty>("Unit info");

   // TODO: переделать шип инфо
   //CRSHIPINFO(m_prop_id, colreg::ship_info, m_info, id, ShipInfoFieldIndex::SIFI_ID);
   //CRSHIPINFO(m_prop_length, colreg::ship_info, m_info, length, ShipInfoFieldIndex::SIFI_LENGTH);
   //CRSHIPINFO(m_prop_width, colreg::ship_info, m_info, width, ShipInfoFieldIndex::SIFI_WIDTH);
   //CRSHIPINFO(m_prop_draft, colreg::ship_info, m_info, draft, ShipInfoFieldIndex::SIFI_DRAFT);
   //CRSHIPINFO(m_prop_safety_contour, colreg::ship_info, m_info, safety_contour, ShipInfoFieldIndex::SIFI_SAFETY_CONTOUR);
   //CRSHIPINFO(m_prop_deadweight, colreg::ship_info, m_info, deadweight, ShipInfoFieldIndex::SIFI_DEADWEIGHT);
   //CRSHIPINFO(m_prop_ais, colreg::ship_info, m_info, AIS_status, ShipInfoFieldIndex::SIFI_AIS);
   //CRSHIPINFO(m_prop_telegraph, colreg::ship_info, m_info, telegraph, ShipInfoFieldIndex::SIFI_TELEGRAPH);
   AddChild(m_ship_info_folder.get());
}

void SelectedUnit::mountTrackPointInfo(const surface_simulation::iLayerUnit* selected)
{
   
}

void SelectedUnit::mountSimSettings(const surface_simulation::iLayerUnit* selected)
{

}

void SelectedUnit::mountColregSettings(const surface_simulation::iLayerUnit* selected)
{
   
}

void SelectedUnit::mountSolution(const surface_simulation::iLayerUnit* selected)
{

}

void SelectedUnit::mountModel(const surface_simulation::iLayerUnit* selected)
{

}

void SelectedUnit::Render(render::iRender* renderer)
{
   renderDomain(renderer);
   renderTrack(renderer);
}

void SelectedDrone::renderDomain(render::iRender* renderer) const
{
   const auto* selected = getSelectedUnit();
   RenderDomain(render::FIND_OBJECT_TYPE::FOT_DRONE_DOMAIN, renderer, selected, selected->GetPos(), 0., selected_drone_domain_info);
}

void SelectedDrone::renderTrack(render::iRender* renderer) const
{
   const auto* selected = getSelectedUnit();
   layer_filter_path track_path{
   };
   TrackLayerHelper::SegmentVisibilityChecker segCheck = [track_path](size_t idx)->bool {
      return true;// LayerFiltersManager::GetInstance().IsFilterVisible(track_path);
   };
   TrackLayerHelper::PointVisibilityChecker pntCheck = [track_path](size_t idx, const CG::geo_point& pnt)->bool {
      return true;// LayerFiltersManager::GetInstance().IsFilterVisible(track_path);
   };
   TrackLayerHelper::renderRoute(renderer, selected->GetInfo().id, selected->GetRoute(surface_simulation::ROUTE_TYPE::RT_SOURSE), selected_drone_track_info, surface_simulation::ROUTE_TYPE::RT_SOURSE, segCheck, pntCheck);
}

void SelectedRover::renderDomain(render::iRender* renderer) const
{
   const auto& selected = getSelectedUnit();
   RenderDomain(render::FIND_OBJECT_TYPE::FOT_ROVER_DOMAIN, renderer, selected, selected->GetPos(), 0., selected_rover_domain_info);
}

void SelectedRover::renderTrack(render::iRender* renderer) const
{
   const auto* selected = getSelectedUnit();
   layer_filter_path track_path{
   };
   TrackLayerHelper::SegmentVisibilityChecker segCheck = [track_path](size_t idx)->bool {
      return true;// LayerFiltersManager::GetInstance().IsFilterVisible(track_path);
   };
   TrackLayerHelper::PointVisibilityChecker pntCheck = [track_path](size_t idx, const CG::geo_point& pnt)->bool {
      return true;// LayerFiltersManager::GetInstance().IsFilterVisible(track_path);
   };
   TrackLayerHelper::renderRoute(renderer, selected->GetInfo().id, selected->GetRoute(surface_simulation::ROUTE_TYPE::RT_SOURSE), selected_rover_track_info, surface_simulation::ROUTE_TYPE::RT_SOURSE, segCheck, pntCheck);
}

void SelectedShip::renderDomain(render::iRender* renderer) const
{
   const auto& selected = getSelectedUnit();
   RenderDomain(render::FIND_OBJECT_TYPE::FOT_SHIP_DOMAIN, renderer, selected, selected->GetPos(), 0., selected_ship_domain_info);
}

void SelectedShip::renderTrack(render::iRender* renderer) const
{
   const auto* selected = getSelectedUnit();
   layer_filter_path track_path{
   };
   TrackLayerHelper::SegmentVisibilityChecker segCheck = [track_path](size_t idx)->bool {
      return true;// LayerFiltersManager::GetInstance().IsFilterVisible(track_path);
   };
   TrackLayerHelper::PointVisibilityChecker pntCheck = [track_path](size_t idx, const CG::geo_point& pnt)->bool {
      return true;// LayerFiltersManager::GetInstance().IsFilterVisible(track_path);
   };
   TrackLayerHelper::renderRoute(renderer, selected->GetInfo().id, selected->GetRoute(surface_simulation::ROUTE_TYPE::RT_SOURSE), selected_ship_track_info, surface_simulation::ROUTE_TYPE::RT_SOURSE, segCheck, pntCheck);
}

#undef CRSHIPINFO