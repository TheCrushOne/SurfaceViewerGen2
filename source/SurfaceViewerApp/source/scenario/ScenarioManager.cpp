#include "stdafx.h"
#include "ScenarioManager.h"
//#include "Colreg\Simulator.h"
//#include "TTCG\Common\FileSystem\Path.h"
#include "gui/selection/SelectedObjectManager.h"
//#include "engine\main\dispatcher.h"


void ScenarioManager::Open(const wchar_t* fileName)
{
   _scenarioFile = fileName;
   SelectedObjectManager::GetInstance().Unselect();

   //Dispatcher::GetInstance().LoadScenario(fileName);

   //if (ScenarioDispather::GetInstance().OnScenarioLoad(fileName))
   //{
   //   setState(CSENARIO_STATUS::SS_PAUSE);
   //   SetDebugMode(_debugMode);
   //}
}

void ScenarioManager::setState(CSENARIO_STATUS state, bool force)
{
   //if (!force && _state == state || !simulator::getSimulator())
   //   return;

   //_state = state;
   //ScenarioDispather::GetInstance().OnScenarioStatusChanged(_state);
}

void ScenarioManager::Run()
{
   setState(CSENARIO_STATUS::SS_RUN);
}

void ScenarioManager::Pause()
{
   setState(CSENARIO_STATUS::SS_PAUSE);
}

void ScenarioManager::Restart()
{
   //dispatcher::getDispatcher()->Stop();
   //dispatcher::getDispatcher()->Start();
   //setState(CSENARIO_STATUS::SS_PAUSE, true);
   //ScenarioDispather::GetInstance().OnScenarioTimeChanged(simulator::getSimulator()->GetState().GetTime());
   //SetDebugMode(_debugMode);
}

void ScenarioManager::Stop()
{
   //if (!simulator::getSimulator())
   //   return;
   //simulator::getSimulator()->Reset();
   //setState(CSENARIO_STATUS::SS_NOT_LOADED, true);
}

void ScenarioManager::Step()
{
   //if (!simulator::getSimulator())
   //   return;
   //if (!simulator::simulatorStep())
   //{
   //   printEnentsAndSuggestions();
   //   if (_autoPause)
   //   {
   //      Pause();
   //   }

   //   //printSolutuins();
   //}
   //else
   //   printEnentsAndSuggestions();
   //ScenarioDispather::GetInstance().OnScenarioTimeChanged(simulator::getSimulator()->GetState().GetTime());
}

void ScenarioManager::ReEstimate()
{
   //const auto& simulationState = sim->GetState();
   //const auto navEstimation = simulationState.GetEstimation();
   //if (!navEstimation->GetResult())
   //   return;

   //auto pClusters = navEstimation->GetClusters();
   //for (size_t i = 0; i < pClusters->size; ++i)
   //   pClusters->arr[i]->SetSettings(simulationState.GetSettings());

   //navEstimation->Solve(simulator::getSimulator()->GetState().GetSettings().cooperativeMode);
}