#include "stdafx.h"
#include "ScenarioManager.h"
#include "simulator\simulator.h"
//#include "TTCG\Common\FileSystem\Path.h"
#include "gui/selection/SelectedObjectManager.h"
//#include "engine\main\dispatcher.h"
#include "common/file_storage.h"
#include "gui/user_interface.h"

ScenarioManager::ScenarioManager()
   : m_info({ "127.0.0.1", "8080", "27015", [this](const char* txt) { this->callback(txt); }})
{
   m_info.data_callback_map[transceiver::JsonCommand::JC_NEWSURFACE] = [this](const char* txt) {};
   createTransceiver();

   m_fsm.Create(SVGUtils::CurrentDllPath("FileStorageManager").c_str(), "CreateConverter");
   if (!m_fsm.IsValid())
   {
      user_interface::SetOutputText(user_interface::OT_ERROR, "Can't load 'HeightMapToSVGMConverter'!");
      return;
   }

   m_converter.Create(SVGUtils::CurrentDllPath("HeightMapToSVGMConverter").c_str(), "CreateConverter");
   if (!m_converter.IsValid())
   {
      user_interface::SetOutputText(user_interface::OT_ERROR, "Can't load 'HeightMapToSVGMConverter'!");
      return;
   }
}

void ScenarioManager::Open(const wchar_t* fileName)
{
   m_scenarioFile = fileName;
   SelectedObjectManager::GetInstance().Unselect();

   m_fsm->Init(simulator::GetCommunicator());
   m_fsm->PrepareStorage(fileName);

   m_converter->Init(simulator::GetCommunicator());
   m_converter->Convert();
   
   //data_share::share_meta meta{L"file.bin"};

   //m_shareProvider->Share(meta, m_rawdata);
   //m_transceiver->Send(SVGUtils::wstringToString(meta.shared_filename).c_str());

   simulator::simulatorStart(m_fsm->GetPathStorage());
   //Dispatcher::GetInstance().LoadScenario(fileName);

   if (ScenarioDispather::GetInstance().OnScenarioLoad(m_fsm->GetPathStorage()))
   {
      setState(CSENARIO_STATUS::SS_PAUSE);
      //SetDebugMode(_debugMode);
   }
}

void ScenarioManager::setState(CSENARIO_STATUS state, bool force)
{
   if (!force && m_state == state || !simulator::getSimulator())
      return;

   m_state = state;
   ScenarioDispather::GetInstance().OnScenarioStatusChanged(m_state);
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
   auto sim = simulator::getSimulator();
   if (!sim)
      return;
   sim->Stop();
   sim->Start();
   setState(CSENARIO_STATUS::SS_PAUSE, true);
   ScenarioDispather::GetInstance().OnScenarioTimeChanged(sim->GetState().GetTime());
   SetDebugMode(m_debugMode);
}

void ScenarioManager::Stop()
{
   auto sim = simulator::getSimulator();
   if (!sim)
      return;
   sim->Reset();
   setState(CSENARIO_STATUS::SS_NOT_LOADED, true);
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
   m_transceiver.Create(SVGUtils::CurrentDllPath("SocketTransceiverLine").c_str(), "CreateTransceiver");
   if (!m_transceiver.IsValid())
   {
      user_interface::SetOutputText(user_interface::OT_ERROR, "Can't load 'SocketTransceiverLine'!");
      return;
   }
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