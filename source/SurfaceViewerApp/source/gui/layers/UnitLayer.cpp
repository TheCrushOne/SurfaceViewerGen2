#include "stdafx.h"
#include "UnitLayer.h"
#include "simulator/simulator.h"
#include "RenderHelper.h"

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
   if (!sim || sim->GetSimulatorScenarioState() == ColregSimulation::SCENARIO_STATUS::SS_NOT_LOADED)
      return;
   const auto& simulationState = sim->GetState();

   for (size_t idx = 0; idx < simulationState.GetUnitCount(ColregSimulation::UNIT_TYPE::UT_ROVER); idx++)
   {
      const auto& rover = simulationState.GetUnitByIdx(ColregSimulation::UNIT_TYPE::UT_ROVER, idx);
      //const auto& placement = simulationState.GetCurrentUnitPositionByIdx(ColregSimulation::UNIT_TYPE::UT_ROVER, idx);

      // TODO: присобачить видимость
      //if (!ScenarioManager::GetInstance().GetShipVisibility(ship.GetInfo().id))
         //continue;
      if (renderer->IsNeedRender({ rover.pos }))
      {
         SVCG::ship_info spike;
         renderer->AddObject({ { rover.pos }, { 2, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(0,0,0), /*rover->GetInfo()*/spike.name, 0, 25} });
         RenderUnitContour(renderer, ColregSimulation::UNIT_TYPE::UT_ROVER, spike/*rover->GetInfo()*/, rover, { 1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_SOLID, RGB(75, 100, 155) });
         //RenderDomain(render::FIND_OBJECT_TYPE::FOT_ROVER_DOMAIN, renderer, rover, rover, 0., { 2, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(75, 100, 155) });
      }

      if (m_renderVector)
      {
         //auto dist = rover.GetPos().point.speed / 6;
         auto course = rover.heading;
         auto pt = math::calc_point(rover.pos, m_arrowDist, course);
         render::object_info objectInfo = { 3, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(163,73,164) };
         renderer->AddObject({ { rover.pos, pt }, objectInfo });
         RenderArrow(renderer, pt, course, objectInfo, 1.);
      }
   }
}

void UnitLayer::renderDrones(render::iRender* renderer)
{
   const auto* sim = simulator::getSimulator();
   if (!sim || sim->GetSimulatorScenarioState() == ColregSimulation::SCENARIO_STATUS::SS_NOT_LOADED)
      return;
   const auto& simulationState = sim->GetState();

   for (size_t idx = 0; idx < simulationState.GetUnitCount(ColregSimulation::UNIT_TYPE::UT_DRONE); idx++)
   {
      const auto& drone = simulationState.GetUnitByIdx(ColregSimulation::UNIT_TYPE::UT_DRONE, idx);
      //const auto& center = drone->GetPos().point.pos;

      // TODO: присобачить видимость
      //if (!ScenarioManager::GetInstance().GetShipVisibility(ship.GetInfo().id))
         //continue;
      if (renderer->IsNeedRender({ drone.pos }))
      {
         SVCG::ship_info spike;
         renderer->AddObject({ { drone.pos }, { 2, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(0,0,0), /*drone->GetInfo()*/spike.name, 0, 25} });
         RenderUnitContour(renderer, ColregSimulation::UNIT_TYPE::UT_DRONE, spike/*drone->GetInfo()*/, drone, { 1, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_SOLID, RGB(75, 100, 155) });
         //RenderDomain(render::FIND_OBJECT_TYPE::FOT_DRONE_DOMAIN, renderer, drone, drone->GetPos().point, 0., { 2, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(75, 100, 155) });
      }

      if (m_renderVector)
      {
         // NOTE: нам пока что пофигу на скорость(т.к. статика)
         //auto dist = drone.GetPos().point.speed / 6;
         auto course = drone.heading;
         auto pt = math::calc_point(drone.pos, m_arrowDist, course);
         render::object_info objectInfo = { 3, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(163,73,164) };
         renderer->AddObject({ { drone.pos, pt }, objectInfo });
         RenderArrow(renderer, pt, course, objectInfo, 1.);
      }
   }
}

void UnitLayer::renderShips(render::iRender* renderer)
{
}