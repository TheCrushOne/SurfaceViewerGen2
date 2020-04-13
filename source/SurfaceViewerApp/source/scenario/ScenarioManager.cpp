#include "stdafx.h"
#include "ScenarioManager.h"
#include "gui/selection/SelectedObjectManager.h"
#include "common/file_storage.h"
#include "gui/user_interface.h"

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName); \
   if (!guard.IsValid()) \
   { \
      user_interface::RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      user_interface::SetOutputText(user_interface::OT_ERROR, errMsg.c_str()); \
      return; \
   }// \
   //guard->Init(GetPack());

ScenarioManager::ScenarioManager()
   : m_info({ "127.0.0.1", "8080", "27015", [this](const char* txt) { this->callback(txt); }})
{
   m_info.data_callback_map[transceiver::JsonCommand::JC_NEWSURFACE] = [this](const char* txt) {};
   // TODO: включить, когда будет более €сна€ картина по протоколу обмена данными
   //createTransceiver();

   VALID_CHECK_DLL_LOAD("FileStorageManager", "CreateFileStorageManager", m_fsm);
   VALID_CHECK_DLL_LOAD("HeightMapToSVGMConverter", "CreateConverter", m_converter);
}

void ScenarioManager::Open(const wchar_t* fileName)
{
   m_scenarioFile = fileName;
   SelectedObjectManager::GetInstance().Unselect();

   m_fsm->Init(simulator::GetPack());
   m_fsm->PrepareStorage(fileName);

   m_converter->Init(simulator::GetPack());
   m_converter->Convert();
   
   //data_share::share_meta meta{L"file.bin"};

   //m_shareProvider->Share(meta, m_rawdata);
   //m_transceiver->Send(SVGUtils::wstringToString(meta.shared_filename).c_str());

   simulator::simulatorStart();
   //Dispatcher::GetInstance().LoadScenario(fileName);

   if (ScenarioDispather::GetInstance().OnScenarioLoad())
   {
      //setState(ColregSimulation::SCENARIO_STATUS::SS_PAUSE);
      //SetDebugMode(_debugMode);
   }
}

void ScenarioManager::setState(ColregSimulation::SCENARIO_STATUS state, bool force)
{
   // TODO: разобратьс€ с force
   if (!simulator::getSimulator())
      return;
   simulator::getSimulator()->SetSimulatorScenarioState(state);
   ScenarioDispather::GetInstance().OnScenarioStatusChanged(state);
}

void ScenarioManager::Run()
{
   setState(ColregSimulation::SCENARIO_STATUS::SS_RUN);
}

void ScenarioManager::Pause()
{
   setState(ColregSimulation::SCENARIO_STATUS::SS_PAUSE);
}

void ScenarioManager::Restart()
{
   auto sim = simulator::getSimulator();
   if (!sim)
      return;
   sim->Stop();
   sim->Start();
   setState(ColregSimulation::SCENARIO_STATUS::SS_PAUSE, true);
   ScenarioDispather::GetInstance().OnScenarioTimeChanged(sim->GetState().GetTime());
   SetDebugMode(m_debugMode);
}

void ScenarioManager::Stop()
{
   auto sim = simulator::getSimulator();
   if (!sim)
      return;
   sim->Reset();
   setState(ColregSimulation::SCENARIO_STATUS::SS_NOT_LOADED, true);
}

void ScenarioManager::Step()
{
   if (!simulator::getSimulator())
      return;
   if (!simulator::simulatorStep())
   {
      //printEnentsAndSuggestions();
      if (m_autoPause)
      {
         Pause();
      }

      //printSolutuins();
   }
   //else
      //printEnentsAndSuggestions();
   ScenarioDispather::GetInstance().OnScenarioTimeChanged(simulator::getSimulator()->GetState().GetTime());
}

void ScenarioManager::ReEstimate()
{
   auto sim = simulator::getSimulator();
   if (!sim)
      return;
   sim->RecountRoutes();
   //const auto& simulationState = sim->GetState();
   //const auto navEstimation = simulationState.GetEstimation();
   //if (!navEstimation->GetResult())
   //   return;

   //auto pClusters = navEstimation->GetClusters();
   //for (size_t i = 0; i < pClusters->size; ++i)
   //   pClusters->arr[i]->SetSettings(simulationState.GetSettings());

   //navEstimation->Solve(simulator::getSimulator()->GetState().GetSettings().cooperativeMode);
}

void ScenarioManager::ReSearch()
{
   auto sim = simulator::getSimulator();
   if (!sim)
      return;
   sim->RecountResearch();
}

void ScenarioManager::LogResearchResult()
{
   auto sim = simulator::getSimulator();
   if (!sim)
      return;
}

void ScenarioManager::createTransceiver()
{
   VALID_CHECK_DLL_LOAD("SocketTransceiverLine", "CreateTransceiver", m_transceiver);
   std::thread thr(&ScenarioManager::initTransceiver, this);
   thr.detach();
   return;
}

// TODO: сделать!!!
void ScenarioManager::SetDebugMode(bool debug)
{}

void ScenarioManager::initTransceiver()
{
   m_transceiver->Init(m_info);
}

void ScenarioManager::callback(const char* text)
{
   user_interface::SetOutputText(user_interface::OT_INFO, text);
}