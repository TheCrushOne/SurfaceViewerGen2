#include "stdafx.h"
#include "UnitLayer.h"
#include "simulator/simulator.h"
#include "RenderHelper.h"

using namespace SV;

namespace
{
   UnitLayer layer;
   //const auto imageSuggestion = fpath::get_module_folder().append("images\\tip.png");
}

//std::string get_suggestion_text(const colreg::statefull::suggestion& suggestion);

void UnitLayer::Render(render::iRender* renderer)
{
   renderRovers(renderer);
   renderDrones(renderer);
   // NOTE: Заготовка
   renderShips(renderer);
}

void UnitLayer::renderRovers(render::iRender* renderer)
{
   const auto* sim = simulator::getSimulator();
   if (!sim || sim->GetSimulatorScenarioState() == surface_simulation::SCENARIO_STATUS::SS_NOT_LOADED)
      return;
   const auto& simulationState = sim->GetState();

   for (size_t idx = 0; idx < simulationState.GetUnitCount(surface_simulation::UNIT_TYPE::UT_ROVER); idx++)
   {
      const auto* rover = simulationState.GetUnitByIdx(surface_simulation::UNIT_TYPE::UT_ROVER, idx);
      //const auto& placement = simulationState.GetCurrentUnitPositionByIdx(surface_simulation::UNIT_TYPE::UT_ROVER, idx);

      // TODO: присобачить видимость
      //if (!ScenarioManager::GetInstance().GetShipVisibility(ship.GetInfo().id))
         //continue;
      auto rover_id = rover->GetInfo().id;

      layer_filter_path id_rover_path = formatDroneLayerFilterPath(rover_id);

      if (LayerFiltersManager::GetInstance().IsFilterVisible(id_rover_path))
      {
         if (renderer->IsNeedRender({ rover->GetPos() }))
         {
            layer_provider::ship_info spike;
            renderer->AddObject({
               { rover->GetPos() },
               { 2, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(0,0,0), rover->GetInfo().name, 0, 25}
               });
            RenderUnitContour(renderer, surface_simulation::UNIT_TYPE::UT_ROVER, rover->GetInfo(), rover->GetPos(), { 1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_SOLID, RGB(75, 100, 155) });
            //RenderDomain(render::FIND_OBJECT_TYPE::FOT_ROVER_DOMAIN, renderer, rover, rover, 0., { 2, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(75, 100, 155) });
         }

         if (m_renderVector)
         {
            //auto dist = rover.GetPos().point.speed / 6;
            auto course = rover->GetPos().heading;
            auto pt = math::calc_point(rover->GetPos(), m_arrowDist, course);
            render::object_info objectInfo = { 3, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(163,73,164) };
            renderer->AddObject({
               { rover->GetPos(), pt },
               objectInfo
               });
            RenderArrow(renderer, pt, course, objectInfo, 1.);
         }
      }
   }
}

void UnitLayer::renderDrones(render::iRender* renderer)
{
   const auto* sim = simulator::getSimulator();
   if (!sim || sim->GetSimulatorScenarioState() == surface_simulation::SCENARIO_STATUS::SS_NOT_LOADED)
      return;
   const auto& simulationState = sim->GetState();

   for (size_t idx = 0; idx < simulationState.GetUnitCount(surface_simulation::UNIT_TYPE::UT_DRONE); idx++)
   {
      const auto& drone = simulationState.GetUnitByIdx(surface_simulation::UNIT_TYPE::UT_DRONE, idx);
      //const auto& center = drone->GetPos().point.pos;

      // TODO: присобачить видимость
      //if (!ScenarioManager::GetInstance().GetShipVisibility(ship.GetInfo().id))
         //continue;
      auto drone_id = drone->GetInfo().id;

      layer_filter_path id_drone_path = formatDroneLayerFilterPath(drone_id);

      if (LayerFiltersManager::GetInstance().IsFilterVisible(id_drone_path))
      {
         if (renderer->IsNeedRender({ drone->GetPos() }))
         {
            renderer->AddObject({
               { drone->GetPos() },
               { 2, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(0,0,0), drone->GetInfo().name, 0, 25}
               });
            RenderUnitContour(renderer, surface_simulation::UNIT_TYPE::UT_DRONE, drone->GetInfo(), drone->GetPos(), { 1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_SOLID, RGB(75, 100, 155) });
            //RenderDomain(render::FIND_OBJECT_TYPE::FOT_DRONE_DOMAIN, renderer, drone, drone->GetPos().point, 0., { 2, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(75, 100, 155) });
         }

         if (m_renderVector)
         {
            // NOTE: нам пока что пофигу на скорость(т.к. статика)
            //auto dist = drone.GetPos().point.speed / 6;
            auto course = drone->GetPos().heading;
            auto pt = math::calc_point(drone->GetPos(), m_arrowDist, course);
            render::object_info objectInfo = { 3, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(163,73,164) };
            renderer->AddObject({
               { drone->GetPos(), pt },
               objectInfo
               });
            RenderArrow(renderer, pt, course, objectInfo, 1.);
         }
      }
   }
}

void UnitLayer::renderShips(render::iRender* renderer)
{
}

bool UnitLayer::onScenarioPathFound()
{
   return onAnyPathFound();
}

bool UnitLayer::onScenarioOptPathFound()
{
   return onAnyPathFound();
}

bool UnitLayer::onAnyPathFound()
{
   auto* sim = simulator::getSimulator();
   if (!sim || sim->GetSimulatorScenarioState() == surface_simulation::SCENARIO_STATUS::SS_NOT_LOADED)
      return false;
   const auto& simulationState = sim->GetState();

   for (size_t idx = 0; idx < simulationState.GetUnitCount(surface_simulation::UNIT_TYPE::UT_ROVER); ++idx)
   {
      const auto* rover = simulationState.GetUnitByIdx(surface_simulation::UNIT_TYPE::UT_ROVER, idx);
      ATLASSERT(rover);

      auto rover_id = rover->GetInfo().id;

      layer_filter_path id_rover_path = formatRoverLayerFilterPath(rover_id);

      sim->AddLayerVisibilityInfoUnit(id_rover_path, true);
   }

   for (size_t idx = 0; idx < simulationState.GetUnitCount(surface_simulation::UNIT_TYPE::UT_DRONE); ++idx)
   {
      const auto* drone = simulationState.GetUnitByIdx(surface_simulation::UNIT_TYPE::UT_DRONE, idx);
      ATLASSERT(drone);

      auto drone_id = drone->GetInfo().id;

      layer_filter_path id_drone_path = formatDroneLayerFilterPath(drone_id);

      sim->AddLayerVisibilityInfoUnit(id_drone_path, true);
   }
}

layer_filter_path UnitLayer::formatDroneLayerFilterPath(id_type id)
{
   return layer_filter_path{
      layer_filter_tag::drones,
      layer_filter_tag::drone_templ + std::to_string(id)
   };
}
layer_filter_path UnitLayer::formatRoverLayerFilterPath(id_type id)
{
   return layer_filter_path{
      layer_filter_tag::rovers,
      layer_filter_tag::rover_templ + std::to_string(id)
   };
}