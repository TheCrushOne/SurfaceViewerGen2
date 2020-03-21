#include "stdafx.h"
#include "SimulatorManager.h"
#include "crossdllinterface/SimulationInterface.h"
#include "RobotLogPlayer/RobotLogPlayer.h"
#include "RobotScenarioPlayer/RobotScenarioPlayer.h"

using namespace ColregSimulation;

void SimulatorManager::Init(ICommunicator* pCommunicator, iPropertyInterface* prop)
{
   m_communicator = pCommunicator;
   m_prop = prop;
}

ColregSimulation::iSimulator* SimulatorManager::Get(const file_utils::global_path_storage& paths)
{
   m_paths = paths;
   // TODO: реализовать проверку через базу по настройкам
   auto type = SIMULATION_PLAYER_TYPE::SPT_SCENARIO;
   size_t stType = static_cast<size_t>(type);
   m_sims[stType] = createSimulationPlayer(type);
   if (!m_sims[stType]->Init(paths))
      return nullptr;
   return m_sims[stType].get();
}

iSimulatorPtr SimulatorManager::createSimulationPlayer(SIMULATION_PLAYER_TYPE type)
{
   auto deleterFun = [](auto* s) { s->Release(); };
   SimulatorBase* sim = nullptr;
   switch (type)
   {
   case SIMULATION_PLAYER_TYPE::SPT_SCENARIO:
      sim = new ColregSimulation::RobotScenarioPlayer(m_communicator, m_prop);
      break;
   case SIMULATION_PLAYER_TYPE::SPT_LOG:
      sim = new ColregSimulation::RobotLogPlayer(m_communicator, m_prop);
      break;
   default:
      ATLASSERT(false);
   }
   sim->SetSimulationType(type);
   ATLASSERT(sim && "ColregSimulation type isn't implemented");
   return iSimulatorPtr(sim ? sim : nullptr, deleterFun);
}

extern "C" SIMEXPRTIMPRT ColregSimulation::iSimulatorManager * __cdecl CreateSimulationManager()
{
   return new SimulatorManager{};
}