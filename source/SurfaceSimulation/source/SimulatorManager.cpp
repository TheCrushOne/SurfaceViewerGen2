#include "stdafx.h"
#include "SimulatorManager.h"
#include "crossdllinterface/SimulationInterface.h"
#include "RobotLogPlayer/RobotLogPlayer.h"
#include "RobotScenarioPlayer/RobotScenarioPlayer.h"

using namespace SV;
using namespace SV::surface_simulation;

void SimulatorManager::SetPropertyInterface(iPropertyInterface* prop)
{
   m_prop = prop;
}

surface_simulation::iSimulator* SimulatorManager::Get(navigation_dispatcher::iComService* service)
{
   // TODO: реализовать проверку через базу по настройкам
   auto type = SIMULATION_PLAYER_TYPE::SPT_SCENARIO;
   size_t stType = static_cast<size_t>(type);
   m_sims[stType] = createSimulationPlayer(type, service);
   //if (!m_sims[stType]->Init(GetPack()))
      //return nullptr;
   return m_sims[stType].get();
}

iSimulatorPtr SimulatorManager::createSimulationPlayer(SIMULATION_PLAYER_TYPE type, navigation_dispatcher::iComService* service)
{
   auto deleterFun = [](auto* s) { s->Release(); };
   SimulatorBase* sim = nullptr;
   switch (type)
   {
   case SIMULATION_PLAYER_TYPE::SPT_SCENARIO:
      sim = new surface_simulation::RobotScenarioPlayer(GetPack(), m_prop, service);
      break;
   case SIMULATION_PLAYER_TYPE::SPT_LOG:
      sim = new surface_simulation::RobotLogPlayer(GetPack(), m_prop, service);
      break;
   default:
      ATLASSERT(false);
   }
   //sim->Init(GetPack());
   sim->SetSimulationType(type);
   ATLASSERT(sim && "surface_simulation type isn't implemented");
   return iSimulatorPtr(sim ? sim : nullptr, deleterFun);
}

extern "C" SIMEXPRTIMPRT surface_simulation::iSimulatorManager * __cdecl CreateSimulationManager(central_pack * pack)
{
   return new SimulatorManager(pack);
}