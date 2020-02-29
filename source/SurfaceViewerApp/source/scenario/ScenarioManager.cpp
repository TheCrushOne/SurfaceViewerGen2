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
}

void ScenarioManager::Open(const wchar_t* fileName)
{
   _scenarioFile = fileName;
   SelectedObjectManager::GetInstance().Unselect();

   if (!m_converter.IsValid())
   {
      m_converter.Create(SVGUtils::CurrentDllPath("HeightMapToSVGMConverter").c_str(), "CreateConverter");
      if (!m_converter.IsValid())
      {
         //MessageString(ICommunicator::MS_Error, "Can't load settings serializer!");
         return;
      }
      //return;
   }
   

   auto hmFS = file_utils::heightmap_file_storage(fileName);
   auto sdFS = file_utils::sqlite_database_file_storage(fileName);

   m_converter->Convert(hmFS, sdFS);
   
   //data_share::share_meta meta{L"file.bin"};

   //m_shareProvider->Share(meta, m_rawdata);
   //m_transceiver->Send(SVGUtils::wstringToString(meta.shared_filename).c_str());

   simulator::simulatorStart(sdFS);
   //Dispatcher::GetInstance().LoadScenario(fileName);

   if (ScenarioDispather::GetInstance().OnScenarioLoad(sdFS))
   {
      setState(CSENARIO_STATUS::SS_PAUSE);
      //SetDebugMode(_debugMode);
   }
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

void ScenarioManager::createTransceiver()
{
   m_transceiver.Create(SVGUtils::CurrentDllPath("SocketTransceiverLine").c_str(), "CreateTransceiver");
   if (!m_transceiver.IsValid())
   {
      user_interface::SetOutputText(user_interface::OT_ERROR, "Can't load Socket Transceiver Line!");
      return;
   }
   std::thread thr(&ScenarioManager::initTransceiver, this);
   thr.detach();
   return;
}

void ScenarioManager::initTransceiver()
{
   m_transceiver->Init(m_info);
}

void ScenarioManager::callback(const char* text)
{
   user_interface::SetOutputText(user_interface::OT_INFO, text);
}