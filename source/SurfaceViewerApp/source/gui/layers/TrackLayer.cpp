#include "stdafx.h"
#include "TrackLayer.h"
#include "simulator/simulator.h"
#include "TrackLayerHelper.h"

using namespace SV;

namespace
{
   TrackLayer layer;
   render::object_info roverRouteObjInfo
   {
      1,
      render::LINE_STYLE::LL_DASH,
      render::FILL_TYPE::FT_NONE,
      RGB(110, 110, 110)
   };
   render::object_info roverControlPointsObjInfo
   {
      1,
      render::LINE_STYLE::LL_NONE,
      render::FILL_TYPE::FT_NONE,
      RGB(150, 150, 150)
   };
   render::object_info droneRouteObjInfo
   {
      1,
      render::LINE_STYLE::LL_DASH,
      render::FILL_TYPE::FT_NONE,
      RGB(255, 200, 30)
   };
   render::object_info droneControlPointsObjInfo
   {
      1,
      render::LINE_STYLE::LL_NONE,
      render::FILL_TYPE::FT_NONE,
      RGB(255, 255, 30)
   };
}
void TrackLayer::renderTracks(render::iRender* renderer)
{
   const auto* sim = simulator::getSimulator();
   if (!sim || sim->GetSimulatorScenarioState() == surface_simulation::SCENARIO_STATUS::SS_NOT_LOADED)
      return;
   const auto& simulationState = sim->GetState();

   for (size_t idx = 0; idx < simulationState.GetUnitCount(surface_simulation::UNIT_TYPE::UT_ROVER); ++idx)
   {
      const auto* rover = simulationState.GetUnitByIdx(surface_simulation::UNIT_TYPE::UT_ROVER, idx);
      ATLASSERT(rover);

      if (m_renderSourceRoute)
         TrackLayerHelper::renderRoute(renderer, rover->id, rover->trajectory, roverRouteObjInfo, surface_simulation::ROUTE_TYPE::RT_SOURSE);
      if (m_renderControlPoints)
         TrackLayerHelper::renderRoute(renderer, rover->id, rover->control_points, roverControlPointsObjInfo, surface_simulation::ROUTE_TYPE::RT_CONTROL);
   }

   for (size_t idx = 0; idx < simulationState.GetUnitCount(surface_simulation::UNIT_TYPE::UT_DRONE); ++idx)
   {
      const auto* drone = simulationState.GetUnitByIdx(surface_simulation::UNIT_TYPE::UT_DRONE, idx);

      if (m_renderSourceRoute)
         TrackLayerHelper::renderRoute(renderer, drone->id, drone->trajectory, droneRouteObjInfo, surface_simulation::ROUTE_TYPE::RT_SOURSE);
      if (m_renderControlPoints)
         TrackLayerHelper::renderRoute(renderer, drone->id, drone->control_points, droneControlPointsObjInfo, surface_simulation::ROUTE_TYPE::RT_CONTROL);
   }
}
