#include "stdafx.h"
#include "SimulatorManager.h"
#include "crossdllinterface/SimulationInterface.h"
#include "RobotLogPlayer/RobotLogPlayer.h"
#include "RobotScenarioPlayer/RobotScenarioPlayer.h"

using namespace ColregSimulation;

void SimulatorManager::SetPropertyInterface(iPropertyInterface* prop)
{
   m_prop = prop;
}

ColregSimulation::iSimulator* SimulatorManager::Get(navigation_dispatcher::iComServicePtr service)
{
   // TODO: реализовать проверку через базу по настройкам
   auto type = SIMULATION_PLAYER_TYPE::SPT_SCENARIO;
   size_t stType = static_cast<size_t>(type);
   m_sims[stType] = createSimulationPlayer(type, service);
   //if (!m_sims[stType]->Init(GetPack()))
      //return nullptr;
   return m_sims[stType].get();
}

iSimulatorPtr SimulatorManager::createSimulationPlayer(SIMULATION_PLAYER_TYPE type, navigation_dispatcher::iComServicePtr service)
{
   auto deleterFun = [](auto* s) { s->Release(); };
   SimulatorBase* sim = nullptr;
   switch (type)
   {
   case SIMULATION_PLAYER_TYPE::SPT_SCENARIO:
      sim = new ColregSimulation::RobotScenarioPlayer(GetPack(), m_prop, service);
      break;
   case SIMULATION_PLAYER_TYPE::SPT_LOG:
      sim = new ColregSimulation::RobotLogPlayer(GetPack(), m_prop, service);
      break;
   default:
      ATLASSERT(false);
   }
   //sim->Init(GetPack());
   sim->SetSimulationType(type);
   ATLASSERT(sim && "ColregSimulation type isn't implemented");
   return iSimulatorPtr(sim ? sim : nullptr, deleterFun);
}

extern "C" SIMEXPRTIMPRT ColregSimulation::iSimulatorManager * __cdecl CreateSimulationManager(central_pack * pack)
{
   return new SimulatorManager(pack);
}