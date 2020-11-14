#include "stdafx.h"
#include "ScenarioManager.h"
#include "gui/selection/SelectedObjectManager.h"
#include "common/file_storage.h"
#include "gui/user_interface.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <regex>

using namespace SV;

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard, ...) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName, __VA_ARGS__); \
   if (!guard.IsValid()) \
   { \
      user_interface::RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      user_interface::SetOutputText(user_interface::OT_ERROR, errMsg.c_str()); \
      return; \
   }// \
   //guard->Init(GetPack());

ScenarioManager::ScenarioManager(central_pack* pack)
   : m_info({ "127.0.0.1", "8080", "27015", [this](const char* txt) { this->callback(txt); }})
{
   m_info.data_callback_map[transceiver::JsonCommand::JC_NEWSURFACE] = [this](const char* txt) {};
   m_cacheFolder = "../../../cache";
   m_databaseFolder = "../../../database";
   // TODO: включить, когда будет более ясная картина по протоколу обмена данными
   //createTransceiver();
   VALID_CHECK_DLL_LOAD("FileStorageManager", "CreateFileStorageManager", m_fsm);
   VALID_CHECK_DLL_LOAD("SurfaceViewerOrderingWrapper", "CreateSurfaceViewerOrderingWrapper", m_orderingWrapper, pack, m_databaseFolder.c_str());
   VALID_CHECK_DLL_LOAD("NavigationDispatcher", "CreateExternalComService", m_comService, pack, m_databaseFolder.c_str());
   //VALID_CHECK_DLL_LOAD("NavigationDispatcher", "CreateNavigationDispatcher", m_navigationDispatcher, pack);
}

void ScenarioManager::CheckOpen(const char* fileName, std::function<void(void)> buttonEnableCallback)
{
   m_pathStorage = file_utils::global_path_storage(fileName);

   SelectedObjectManager::GetInstance().Unselect();
   // TODO: дописать чек
   if (true)
   {
      simulator::simulatorInit(m_comService.operator->());
      auto& settings = m_comService->GetSettingsSerializerHolder()->GetSettings(m_pathStorage);
      simulator::getSimulator()->SetAppSettings(settings);
      simulator::getSimulator()->CheckOpenScenario();
      ScenarioDispather::GetInstance().OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS::SS_MAP_CHECKOPENED);
      buttonEnableCallback();
   }
}

void ScenarioManager::ProcessMap(std::function<void(void)> buttonEnableCallback)
{
   std::thread(&ScenarioManager::processMapCommand, this, [this, buttonEnableCallback]()
      {
         //auto& settings = m_comService->GetSettingsSerializerHolder()->GetSettings();
         //simulator::getSimulator()->SetAppSettings(settings);
         simulator::getSimulator()->LoadProcessedMap();
         ScenarioDispather::GetInstance().OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS::SS_MAP_PROCESSED);
         buttonEnableCallback();
      }
   ).detach();
}

void ScenarioManager::ProcessMapObjects(std::function<void(void)> buttonEnableCallback)
{
   std::thread(&ScenarioManager::processMapObjCommand, this, [this, buttonEnableCallback]()
      {
         //auto& settings = m_comService->GetSettingsSerializerHolder()->GetSettings();
         //simulator::getSimulator()->SetAppSettings(settings);
         simulator::getSimulator()->LoadProcessedMapObjects();
         ScenarioDispather::GetInstance().OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS::SS_MAPOBJ_PROCESSED);
         buttonEnableCallback();
      }
   ).detach();
}

void ScenarioManager::ProcessPaths(std::function<void(void)> buttonEnableCallback)
{
   std::thread(&ScenarioManager::processPathCommand, this, [this, buttonEnableCallback]()
      {
         //auto& settings = m_comService->GetSettingsSerializerHolder()->GetSettings();
         //simulator::getSimulator()->SetAppSettings(settings);
         simulator::getSimulator()->LoadProcessedPaths();
         ScenarioDispather::GetInstance().OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS::SS_PATHS_COUNTED);
         simulator::simulatorStart();
         buttonEnableCallback();
      }
   ).detach();
}

void ScenarioManager::ProcessOptPaths(std::function<void(void)> buttonEnableCallback)
{
   std::thread(&ScenarioManager::processOptPathCommand, this, [this, buttonEnableCallback]()
      {
         //auto& settings = m_comService->GetSettingsSerializerHolder()->GetSettings();
         //simulator::getSimulator()->SetAppSettings(settings);
         simulator::getSimulator()->LoadProcessedOptPaths();
         ScenarioDispather::GetInstance().OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS::SS_OPT_PATHS_COUNTED);
         simulator::simulatorStart();
         buttonEnableCallback();
      }
   ).detach();
}

void ScenarioManager::processMapCommand(std::function<void(void)> successCallback)
{
   auto wscen = file_utils::getFileName(m_pathStorage.map_object_path.path);
   std::unordered_map<std::string, std::string> dict = {
      { "PNG_SRC", m_pathStorage.map_object_path.path },
      { "SVGM_FLDR", m_cacheFolder + wscen + m_svgmCacheFolder },
      { "UNIT_DATA", m_pathStorage.unit_data_path.path },
      { "ENV_STT", m_pathStorage.environment_settings_path.path },
      { "MAP_STT", m_pathStorage.map_settings_path.path },
      { "PTH_STT", m_pathStorage.pathfinder_settings_path.path },
      { "RES_STT", m_pathStorage.research_settings_path.path },
      { "SIM_STT", m_pathStorage.simulation_settings_path.path }
   };

   m_mapCommandProcessed = m_orderingWrapper->ProcessOrder("process_map.xml", NULL, dict);
   if (m_mapCommandProcessed)
      successCallback();
}

void ScenarioManager::processMapObjCommand(std::function<void(void)> successCallback)
{
   auto wscen = file_utils::getFileName(m_pathStorage.map_object_path.path);
   std::unordered_map<std::string, std::string> dict = {
      { "PNG_SRC", m_pathStorage.map_object_path.path },
      { "SVGM_FLDR", m_cacheFolder + wscen + m_svgmCacheFolder },
      { "UNIT_DATA", m_pathStorage.unit_data_path.path },
      { "ENV_STT", m_pathStorage.environment_settings_path.path },
      { "MAP_STT", m_pathStorage.map_settings_path.path },
      { "PTH_STT", m_pathStorage.pathfinder_settings_path.path },
      { "RES_STT", m_pathStorage.research_settings_path.path },
      { "SIM_STT", m_pathStorage.simulation_settings_path.path },
      { "OBJ_FLDR", m_cacheFolder + wscen + m_objMapCacheFolder }
   };

   m_mapCommandProcessed = m_orderingWrapper->ProcessOrder("process_map_object.xml", NULL, dict);
   if (m_mapCommandProcessed)
      successCallback();
}

void ScenarioManager::processPathCommand(std::function<void(void)> successCallback)
{
   auto wscen = file_utils::getFileName(m_pathStorage.map_object_path.path);
   std::unordered_map<std::string, std::string> dict = {
      { "PNG_SRC", m_pathStorage.map_object_path.path },
      { "SVGM_FLDR", m_cacheFolder + wscen + m_svgmCacheFolder },
      { "UNIT_DATA", m_pathStorage.unit_data_path.path },
      { "ENV_STT", m_pathStorage.environment_settings_path.path },
      { "MAP_STT", m_pathStorage.map_settings_path.path },
      { "PTH_STT", m_pathStorage.pathfinder_settings_path.path },
      { "RES_STT", m_pathStorage.research_settings_path.path },
      { "SIM_STT", m_pathStorage.simulation_settings_path.path },
      { "OBJ_FLDR", m_cacheFolder + wscen + m_objMapCacheFolder },
      { "PATHS_DST", m_cacheFolder + wscen + m_pathsCacheFolder }
   };

   m_pathCommandProcessed = m_orderingWrapper->ProcessOrder("process_path_find.xml", NULL, dict);
   if (m_pathCommandProcessed)
      successCallback();
}

void ScenarioManager::processOptPathCommand(std::function<void(void)> successCallback)
{
   auto wscen = file_utils::getFileName(m_pathStorage.map_object_path.path);
   std::unordered_map<std::string, std::string> dict = {
      { "PNG_SRC", m_pathStorage.map_object_path.path },
      { "SVGM_FLDR", m_cacheFolder + wscen + m_svgmCacheFolder },
      { "UNIT_DATA", m_pathStorage.unit_data_path.path },
      { "ENV_STT", m_pathStorage.environment_settings_path.path },
      { "MAP_STT", m_pathStorage.map_settings_path.path },
      { "PTH_STT", m_pathStorage.pathfinder_settings_path.path },
      { "RES_STT", m_pathStorage.research_settings_path.path },
      { "SIM_STT", m_pathStorage.simulation_settings_path.path },
      { "OBJ_FLDR", m_cacheFolder + wscen + m_objMapCacheFolder },
      { "OPTPATHS_DST", m_cacheFolder + wscen + m_optPathsCacheFolder }
   };

   m_optPathCommandProcessed = m_orderingWrapper->ProcessOrder("process_opt_path_find.xml", NULL, dict);
   if (m_optPathCommandProcessed)
      successCallback();
}

void ScenarioManager::setState(surface_simulation::SIMULATION_STATUS state, bool force)
{
   // TODO: разобраться с force
   if (!simulator::getSimulator())
      return;
   simulator::getSimulator()->SetSimulatorSimulationState(state);
   ScenarioDispather::GetInstance().OnScenarioSimulationStatusChanged(state);
}

void ScenarioManager::Run()
{
   setState(surface_simulation::SIMULATION_STATUS::SS_RUN);
}

void ScenarioManager::Pause()
{
   setState(surface_simulation::SIMULATION_STATUS::SS_PAUSE);
}

void ScenarioManager::Restart()
{
   auto sim = simulator::getSimulator();
   if (!sim)
      return;
   sim->Stop();
   sim->Start();
   setState(surface_simulation::SIMULATION_STATUS::SS_PAUSE, true);
   ScenarioDispather::GetInstance().OnScenarioTimeChanged(sim->GetState().GetTime());
   SetDebugMode(m_debugMode);
}

void ScenarioManager::Stop()
{
   auto sim = simulator::getSimulator();
   if (!sim)
      return;
   sim->Reset();
   setState(surface_simulation::SIMULATION_STATUS::SS_STOP, true);
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