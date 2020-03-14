#include "stdafx.h"
#include "TrackLayer.h"
#include "simulator/simulator.h"
#include "TrackLayerHelper.h"

namespace
{
   TrackLayer layer;
}
void TrackLayer::renderTracks(render::iRender* renderer)
{
   const auto* sim = simulator::getSimulator();
   if (!sim)
      return;
   const auto& simulationState = sim->GetState();

   for (size_t idx = 0; idx < simulationState.GetUnitCount(ColregSimulation::UNIT_TYPE::UT_ROVER); ++idx)
   {
      const auto& rover = simulationState.GetUnit(ColregSimulation::UNIT_TYPE::UT_ROVER, idx);

      if (_renderSourceRoute)
         TrackLayerHelper::renderRoute(renderer, rover.GetInfo().id, rover.GetRoute(ColregSimulation::ROUTE_TYPE::RT_SOURSE)->route, { 1, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, RGB(110, 110, 110) }, ColregSimulation::ROUTE_TYPE::RT_SOURSE);
   }

   for (size_t idx = 0; idx < simulationState.GetUnitCount(ColregSimulation::UNIT_TYPE::UT_DRONE); ++idx)
   {
      const auto& drone = simulationState.GetUnit(ColregSimulation::UNIT_TYPE::UT_DRONE, idx);

      if (_renderSourceRoute)
         TrackLayerHelper::renderRoute(renderer, drone.GetInfo().id, drone.GetRoute(ColregSimulation::ROUTE_TYPE::RT_SOURSE)->route, { 1, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, RGB(255, 200, 30) }, ColregSimulation::ROUTE_TYPE::RT_SOURSE);
   }
}