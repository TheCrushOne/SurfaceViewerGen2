#include "stdafx.h"
#include "UnitLayer.h"
#include "simulator/simulator.h"

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
   if (!sim)
      return;
   const auto& simulationState = sim->GetState();

   for (size_t idx = 0; idx < simulationState.GetUnitCount(ColregSimulation::UNIT_TYPE::UT_ROVER); idx++)
   {
      const auto& rover = simulationState.GetUnit(ColregSimulation::UNIT_TYPE::UT_ROVER, idx);
      const auto& center = rover.GetPos().point.pos;

      // TODO: присобачить видимость
      //if (!ScenarioManager::GetInstance().GetShipVisibility(ship.GetInfo().id))
         //continue;

      if (_renderVector)
      {
         auto dist = rover.GetPos().point.speed / 6; //äèñòàíöèÿ çà 10 ìèíóò
         auto course = rover.GetPos().point.course != colreg::NO_VALUE ? rover.GetPos().point.course : rover.GetPos().point.heading;
         auto pt = math::calc_point(rover.GetPos().point.pos, dist, course);
         render::object_info  objectInfo = { 3, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(163,73,164) };
         renderer->AddObject({ { rover.GetPos().point.pos, pt }, objectInfo });
         RenderArrow(renderer, pt, course, objectInfo, 0.5);
      }
   }
}

void UnitLayer::renderDrones(render::iRender* renderer)
{
}

void UnitLayer::renderShips(render::iRender* renderer)
{
}