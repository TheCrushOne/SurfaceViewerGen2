#include "stdafx.h"
#include "TrackLayer.h"
#include "simulator/simulator.h"
#include "TrackLayerHelper.h"

using namespace SV;

namespace
{
   TrackLayer layer;
   const render::object_info rover_route_obj_info()
   {
      return { 1,
         render::LINE_STYLE::LL_DASH,
         render::FILL_TYPE::FT_NONE,
         RGB(110, 110, 110)
      };
   };

   const render::object_info rover_control_points_obj_info()
   {
      return { 1,
         render::LINE_STYLE::LL_NONE,
         render::FILL_TYPE::FT_NONE,
         RGB(150, 150, 150)
      };
   };

   const render::object_info drone_route_obj_info()
   {
      return { 1,
         render::LINE_STYLE::LL_DASH,
         render::FILL_TYPE::FT_NONE,
         RGB(255, 200, 30)
      };
   };

   const render::object_info drone_control_points_obj_info()
   {
      return { 1,
         render::LINE_STYLE::LL_NONE,
         render::FILL_TYPE::FT_NONE,
         RGB(255, 255, 30)
      };
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
         TrackLayerHelper::renderRoute(renderer, rover->GetInfo().id, rover->GetSrcPath(), rover_route_obj_info(), surface_simulation::ROUTE_TYPE::RT_SOURSE);
      if (m_renderControlPoints)
         TrackLayerHelper::renderRoute(renderer, rover->GetInfo().id, rover->GetSrcControlPoints(), rover_control_points_obj_info(), surface_simulation::ROUTE_TYPE::RT_CONTROL);
   }

   for (size_t idx = 0; idx < simulationState.GetUnitCount(surface_simulation::UNIT_TYPE::UT_DRONE); ++idx)
   {
      const auto* drone = simulationState.GetUnitByIdx(surface_simulation::UNIT_TYPE::UT_DRONE, idx);

      if (m_renderSourceRoute)
         TrackLayerHelper::renderRoute(renderer, drone->GetInfo().id, drone->GetSrcPath(), drone_route_obj_info(), surface_simulation::ROUTE_TYPE::RT_SOURSE);
      if (m_renderControlPoints)
         TrackLayerHelper::renderRoute(renderer, drone->GetInfo().id, drone->GetSrcControlPoints(), drone_control_points_obj_info(), surface_simulation::ROUTE_TYPE::RT_CONTROL);
   }
}
