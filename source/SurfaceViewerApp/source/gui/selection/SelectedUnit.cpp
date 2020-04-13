#include "stdafx.h"
#include "SelectedUnit.h"
#include "simulator/simulator.h"
#include "property_helper.h"
#include "gui/layers/RenderHelper.h"
#include "gui/layers/TrackLayerHelper.h"

#define CRSHIPINFO(iPropPtr, prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(iPropPtr, prStruct, obj, field, SelectedShip::shipInfoMeta, sifi, &SelectedShip::OnShipInfoChanged)

SelectedUnit::SelectedUnit(colreg::id_type id, ColregSimulation::UNIT_TYPE type)
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

const ColregSimulation::iSimulationState& SelectedUnit::getState() const
{
   const auto* sim = simulator::getSimulator();
   return sim->GetState();
}

void SelectedUnit::mountUnitInfo(const ColregSimulation::iUnit* selected)
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

void SelectedUnit::mountTrackPointInfo(const ColregSimulation::iUnit* selected)
{
   
}

void SelectedUnit::mountSimSettings(const ColregSimulation::iUnit* selected)
{

}

void SelectedUnit::mountColregSettings(const ColregSimulation::iUnit* selected)
{
   
}

void SelectedUnit::mountSolution(const ColregSimulation::iUnit* selected)
{

}

void SelectedUnit::mountModel(const ColregSimulation::iUnit* selected)
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
   RenderDomain(render::FIND_OBJECT_TYPE::FOT_DRONE_DOMAIN, renderer, selected, selected->GetPos().point, 0., { 3, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NULL, user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha });
}

void SelectedDrone::renderTrack(render::iRender* renderer) const
{
   const auto* selected = getSelectedUnit();
   TrackLayerHelper::renderRoute(renderer, selected->GetInfo().id, selected->GetRoute(ColregSimulation::ROUTE_TYPE::RT_SOURSE)->route, { 1, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, RGB(110, 20, 20) }, ColregSimulation::ROUTE_TYPE::RT_SOURSE);
}

void SelectedRover::renderDomain(render::iRender* renderer) const
{
   const auto& selected = getSelectedUnit();
   RenderDomain(render::FIND_OBJECT_TYPE::FOT_ROVER_DOMAIN, renderer, selected, selected->GetPos().point, 0., { 3, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NULL, user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha });
}

void SelectedRover::renderTrack(render::iRender* renderer) const
{
   const auto* selected = getSelectedUnit();
   TrackLayerHelper::renderRoute(renderer, selected->GetInfo().id, selected->GetRoute(ColregSimulation::ROUTE_TYPE::RT_SOURSE)->route, { 1, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, RGB(110, 20, 20) }, ColregSimulation::ROUTE_TYPE::RT_SOURSE);
}

void SelectedShip::renderDomain(render::iRender* renderer) const
{
   const auto& selected = getSelectedUnit();
   RenderDomain(render::FIND_OBJECT_TYPE::FOT_SHIP_DOMAIN, renderer, selected, selected->GetPos().point, 0., { 3, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NULL, user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha });
}

void SelectedShip::renderTrack(render::iRender* renderer) const
{
   const auto* selected = getSelectedUnit();
   TrackLayerHelper::renderRoute(renderer, selected->GetInfo().id, selected->GetRoute(ColregSimulation::ROUTE_TYPE::RT_SOURSE)->route, { 1, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, RGB(110, 20, 20) }, ColregSimulation::ROUTE_TYPE::RT_SOURSE);
}