#include "stdafx.h"
#include "ScenarioProperties.h"
#include "simulator/simulator.h"

#define CRSCENPATHSTT(prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(m_prop_pathfinding_settings[static_cast<size_t>(sifi)], prStruct, obj, field, ScenarioProperties::scenarioPathfindingSettingsMeta, sifi, &ScenarioProperties::OnPathfindingSettingChanged)
#define CRSCENRESSTT(prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(m_prop_research_settings[static_cast<size_t>(sifi)], prStruct, obj, field, ScenarioProperties::scenarioResearchSettingsMeta, sifi, &ScenarioProperties::OnResearchSettingChanged)

using namespace SV;

ScenarioProperties::ScenarioProperties()
{
   //const auto* disp = dispatcher::getDispatcher();
   const auto* sim = simulator::getSimulator();
   if (sim)
   {
      //const auto& simulationState = sim->GetState();
      m_settings = sim->GetAppSettings();
      prepareScenarioPathfindingSettingsFolder();
      prepareScenarioResearchSettingsFolder();
   }
}

void ScenarioProperties::prepareScenarioPathfindingSettingsFolder()
{
   const auto* sim = simulator::getSimulator();
   m_scenario_pathfinding_settings_folder = std::make_unique<FolderProperty>("Pathfinding settings");
   m_prop_pathfinding_settings.resize(ScenarioPathfindingSettingsFieldIndex::SPSFI_END);

   m_scenario_pathfinding_settings_level_folder = std::make_unique<FolderProperty>("Level settings");
   CRSCENPATHSTT(settings::level_settings, m_settings.pth_stt.lvl_stt, max_air_height, ScenarioPathfindingSettingsFieldIndex::SPSFI_MAXAIRHEIGHT);
   CRSCENPATHSTT(settings::level_settings, m_settings.pth_stt.lvl_stt, max_land_height, ScenarioPathfindingSettingsFieldIndex::SPSFI_MAXLANDHEIGHT);
   CRSCENPATHSTT(settings::level_settings, m_settings.pth_stt.lvl_stt, min_land_height, ScenarioPathfindingSettingsFieldIndex::SPSFI_MINLANDHEIGHT);
   CRSCENPATHSTT(settings::level_settings, m_settings.pth_stt.lvl_stt, max_land_angle, ScenarioPathfindingSettingsFieldIndex::SPSFI_MAXLANDANGLE);
   CRSCENPATHSTT(settings::level_settings, m_settings.pth_stt.lvl_stt, dangerous_land_angle, ScenarioPathfindingSettingsFieldIndex::SPSFI_DANGEROUSLANDANGLE);

   //_prop_settings[ScenarioSettingsFieldIndex::SSFI_ROUTEUPDATESPEEDSTRATEGY]->set_list(routeSpeedUpdateStrategies);

   for (size_t idx = SPSFI_MAXAIRHEIGHT; idx < m_prop_pathfinding_settings.size(); idx++)
   {
      if (m_prop_pathfinding_settings[idx].get())
         m_scenario_pathfinding_settings_level_folder->AddChild(m_prop_pathfinding_settings[idx].get());
   }

   m_scenario_pathfinding_settings_folder->AddChild(m_scenario_pathfinding_settings_level_folder.get());

   AddChild(m_scenario_pathfinding_settings_folder.get());
}

void ScenarioProperties::prepareScenarioResearchSettingsFolder()
{
   const auto* sim = simulator::getSimulator();
   m_scenario_research_settings_folder = std::make_unique<FolderProperty>("Research settings");
   m_prop_research_settings.resize(ScenarioResearchSettingsFieldIndex::SRSFI_END);

   m_scenario_research_settings_countrange_folder = std::make_unique<FolderProperty>("Count range");
   CRSCENRESSTT(settings::range_data<size_t>, m_settings.res_stt.fly_count_range, min, ScenarioResearchSettingsFieldIndex::SRSFI_COUTNRANGE_MIN);
   CRSCENRESSTT(settings::range_data<size_t>, m_settings.res_stt.fly_count_range, max, ScenarioResearchSettingsFieldIndex::SRSFI_COUTNRANGE_MAX);
   CRSCENRESSTT(settings::range_data<size_t>, m_settings.res_stt.fly_count_range, step, ScenarioResearchSettingsFieldIndex::SRSFI_COUTNRANGE_STEP);
   
   m_scenario_research_settings_lengthrange_folder = std::make_unique<FolderProperty>("Length range");
   CRSCENRESSTT(settings::range_data<double>, m_settings.res_stt.length_range, min, ScenarioResearchSettingsFieldIndex::SRSFI_LEGNTHRANGE_MIN);
   CRSCENRESSTT(settings::range_data<double>, m_settings.res_stt.length_range, max, ScenarioResearchSettingsFieldIndex::SRSFI_LEGNTHRANGE_MAX);
   CRSCENRESSTT(settings::range_data<double>, m_settings.res_stt.length_range, step, ScenarioResearchSettingsFieldIndex::SRSFI_LEGNTHRANGE_STEP);

   CRSCENRESSTT(settings::research_settings, m_settings.res_stt, iter_count, ScenarioResearchSettingsFieldIndex::SRSFI_ITERCOUNT);
   CRSCENRESSTT(settings::research_settings, m_settings.res_stt, map_size, ScenarioResearchSettingsFieldIndex::SRSFI_MAPSIZE);
   CRSCENRESSTT(settings::research_settings, m_settings.res_stt, multi_thread_test, ScenarioResearchSettingsFieldIndex::SRSFI_MULTITHREADTEST);
   CRSCENRESSTT(settings::research_settings, m_settings.res_stt, single_thread_test, ScenarioResearchSettingsFieldIndex::SRSFI_SINGLETHREADTEST);
   CRSCENRESSTT(settings::research_settings, m_settings.res_stt, debug_level, ScenarioResearchSettingsFieldIndex::SRSFI_DEBUGLEVEL);
   m_prop_research_settings[ScenarioResearchSettingsFieldIndex::SRSFI_DEBUGLEVEL]->set_list(debugModes);

   for (size_t idx = SRSFI_ITERCOUNT; idx < SRSFI_END; idx++)
      m_scenario_research_settings_folder->AddChild(m_prop_research_settings[idx].get());

   m_scenario_research_settings_folder->AddChild(m_scenario_research_settings_countrange_folder.get());
   m_scenario_research_settings_folder->AddChild(m_scenario_research_settings_lengthrange_folder.get());

   AddChild(m_scenario_research_settings_folder.get());
}

void ScenarioProperties::OnPathfindingSettingChanged()
{
   //simulator::getSimulator()->SetSettings(_settings);
   //simulator::getSimulator()->ReEstimate();
   //user_interface::InvalidateView();
}

void ScenarioProperties::OnResearchSettingChanged()
{
   //simulator::getSimulator()->SetSettings(_settings);
   //simulator::getSimulator()->ReEstimate();
   //user_interface::InvalidateView();
}

#undef CRSCENPATHSTT
#undef CRSCENRESSTT