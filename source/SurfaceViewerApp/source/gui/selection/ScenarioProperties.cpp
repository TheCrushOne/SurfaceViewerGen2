#include "stdafx.h"
#include "ScenarioProperties.h"
#include "simulator/simulator.h"

#define CR_SCEN_PATH_STT(prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(m_prop_pathfinding_settings[static_cast<size_t>(sifi)], prStruct, obj, field, ScenarioProperties::scenarioPathfindingSettingsMeta, sifi, &ScenarioProperties::OnPathfindingSettingChanged)
#define CR_SCEN_RES_STT(prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(m_prop_research_settings[static_cast<size_t>(sifi)], prStruct, obj, field, ScenarioProperties::scenarioResearchSettingsMeta, sifi, &ScenarioProperties::OnResearchSettingChanged)
#define CR_SCEN_ENV_STT(prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(m_prop_environment_settings[static_cast<size_t>(sifi)], prStruct, obj, field, ScenarioProperties::scenarioEnvironmentSettingsMeta, sifi, &ScenarioProperties::OnEnvironmentSettingChanged)
#define CR_SCEN_SIM_STT(prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(m_prop_simulation_settings[static_cast<size_t>(sifi)], prStruct, obj, field, ScenarioProperties::scenarioSimulationSettingsMeta, sifi, &ScenarioProperties::OnSimulationSettingChanged)
#define CR_SCEN_MAP_STT(prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(m_prop_map_settings[static_cast<size_t>(sifi)], prStruct, obj, field, ScenarioProperties::scenarioMapSettingsMeta, sifi, &ScenarioProperties::OnMapSettingChanged)

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
   m_scenario_pathfinding_settings_folder = std::make_unique<FolderPropertyHolder>("Pathfinding settings");
   m_prop_pathfinding_settings.resize(ScenarioPathfindingSettingsFieldIndex::SPSFI_END);

   m_scenario_pathfinding_settings_level_folder = std::make_unique<FolderPropertyHolder>("Level settings");
   CR_SCEN_PATH_STT(settings::level_settings, m_settings.pth_stt.lvl_stt, max_air_height, ScenarioPathfindingSettingsFieldIndex::SPSFI_MAXAIRHEIGHT);
   CR_SCEN_PATH_STT(settings::level_settings, m_settings.pth_stt.lvl_stt, max_land_height, ScenarioPathfindingSettingsFieldIndex::SPSFI_MAXLANDHEIGHT);
   CR_SCEN_PATH_STT(settings::level_settings, m_settings.pth_stt.lvl_stt, min_land_height, ScenarioPathfindingSettingsFieldIndex::SPSFI_MINLANDHEIGHT);
   CR_SCEN_PATH_STT(settings::level_settings, m_settings.pth_stt.lvl_stt, max_land_angle, ScenarioPathfindingSettingsFieldIndex::SPSFI_MAXLANDANGLE);
   CR_SCEN_PATH_STT(settings::level_settings, m_settings.pth_stt.lvl_stt, dangerous_land_angle, ScenarioPathfindingSettingsFieldIndex::SPSFI_DANGEROUSLANDANGLE);

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
   m_scenario_research_settings_folder = std::make_unique<FolderPropertyHolder>("Research settings");
   m_prop_research_settings.resize(ScenarioResearchSettingsFieldIndex::SRSFI_END);

   m_scenario_research_settings_countrange_folder = std::make_unique<FolderPropertyHolder>("Count range");
   CR_SCEN_RES_STT(settings::range_data<size_t>, m_settings.res_stt.fly_count_range, min_val, ScenarioResearchSettingsFieldIndex::SRSFI_COUTNRANGE_MIN);
   CR_SCEN_RES_STT(settings::range_data<size_t>, m_settings.res_stt.fly_count_range, max_val, ScenarioResearchSettingsFieldIndex::SRSFI_COUTNRANGE_MAX);
   CR_SCEN_RES_STT(settings::range_data<size_t>, m_settings.res_stt.fly_count_range, step, ScenarioResearchSettingsFieldIndex::SRSFI_COUTNRANGE_STEP);
   
   m_scenario_research_settings_lengthrange_folder = std::make_unique<FolderPropertyHolder>("Length range");
   CR_SCEN_RES_STT(settings::range_data<double>, m_settings.res_stt.length_range, min_val, ScenarioResearchSettingsFieldIndex::SRSFI_LEGNTHRANGE_MIN);
   CR_SCEN_RES_STT(settings::range_data<double>, m_settings.res_stt.length_range, max_val, ScenarioResearchSettingsFieldIndex::SRSFI_LEGNTHRANGE_MAX);
   CR_SCEN_RES_STT(settings::range_data<double>, m_settings.res_stt.length_range, step, ScenarioResearchSettingsFieldIndex::SRSFI_LEGNTHRANGE_STEP);

   CR_SCEN_RES_STT(settings::research_settings, m_settings.res_stt, iter_count, ScenarioResearchSettingsFieldIndex::SRSFI_ITERCOUNT);
   CR_SCEN_RES_STT(settings::research_settings, m_settings.res_stt, map_size, ScenarioResearchSettingsFieldIndex::SRSFI_MAPSIZE);
   CR_SCEN_RES_STT(settings::research_settings, m_settings.res_stt, multi_thread_test, ScenarioResearchSettingsFieldIndex::SRSFI_MULTITHREADTEST);
   CR_SCEN_RES_STT(settings::research_settings, m_settings.res_stt, single_thread_test, ScenarioResearchSettingsFieldIndex::SRSFI_SINGLETHREADTEST);
   CR_SCEN_RES_STT(settings::research_settings, m_settings.res_stt, debug_level, ScenarioResearchSettingsFieldIndex::SRSFI_DEBUGLEVEL);
   m_prop_research_settings[ScenarioResearchSettingsFieldIndex::SRSFI_DEBUGLEVEL]->set_list(debugModes);

   for (size_t idx = SRSFI_ITERCOUNT; idx < SRSFI_END; idx++)
   {
      if(m_prop_research_settings[idx].get())
         m_scenario_research_settings_folder->AddChild(m_prop_research_settings[idx].get());
   }

   for (size_t idx = SRSFI_COUTNRANGE_MIN; idx < m_prop_pathfinding_settings.size(); idx++)
   {
      if (m_prop_research_settings[idx].get())
         m_scenario_pathfinding_settings_level_folder->AddChild(m_prop_pathfinding_settings[idx].get());
   }

   m_scenario_research_settings_folder->AddChild(m_scenario_research_settings_countrange_folder.get());
   m_scenario_research_settings_folder->AddChild(m_scenario_research_settings_lengthrange_folder.get());

   AddChild(m_scenario_research_settings_folder.get());
}

void ScenarioProperties::prepareScenarioEnvironmentSettingsFolder()
{
   m_scenario_environment_settings_folder = std::make_unique<FolderPropertyHolder>("Environment settings");
   m_prop_environment_settings.resize(ScenarioEnvironmentSettingsFieldIndex::SESFI_END);

   m_scenario_environment_settings_gcs_folder = std::make_unique<FolderPropertyHolder>("Geographical coordinate system");
   CR_SCEN_ENV_STT(settings::coordinate_system_info, m_settings.env_stt.gcs_info, angle, ScenarioEnvironmentSettingsFieldIndex::SESFI_GCS_ANGLE);
   CR_SCEN_ENV_STT(settings::coordinate_system_info, m_settings.env_stt.gcs_info, scale, ScenarioEnvironmentSettingsFieldIndex::SESFI_GCS_SCALE);
   CR_SCEN_ENV_STT(settings::coordinate_system_info, m_settings.env_stt.gcs_info, ordinate_bias, ScenarioEnvironmentSettingsFieldIndex::SESFI_GCS_ORD_BIAS);
   CR_SCEN_ENV_STT(settings::coordinate_system_info, m_settings.env_stt.gcs_info, abscissa_bias, ScenarioEnvironmentSettingsFieldIndex::SESFI_GCS_ABS_BIAS);

   m_scenario_environment_settings_mtx_folder = std::make_unique<FolderPropertyHolder>("Matrix coordinate system");
   CR_SCEN_ENV_STT(settings::coordinate_system_info, m_settings.env_stt.mtx_info, angle, ScenarioEnvironmentSettingsFieldIndex::SESFI_MTX_ANGLE);
   CR_SCEN_ENV_STT(settings::coordinate_system_info, m_settings.env_stt.mtx_info, scale, ScenarioEnvironmentSettingsFieldIndex::SESFI_MTX_SCALE);
   CR_SCEN_ENV_STT(settings::coordinate_system_info, m_settings.env_stt.mtx_info, ordinate_bias, ScenarioEnvironmentSettingsFieldIndex::SESFI_MTX_ORD_BIAS);
   CR_SCEN_ENV_STT(settings::coordinate_system_info, m_settings.env_stt.mtx_info, abscissa_bias, ScenarioEnvironmentSettingsFieldIndex::SESFI_MTX_ABS_BIAS);

   m_scenario_environment_settings_folder->AddChild(m_scenario_environment_settings_gcs_folder.get());
   m_scenario_environment_settings_folder->AddChild(m_scenario_environment_settings_mtx_folder.get());

   AddChild(m_scenario_environment_settings_folder.get());
}

void ScenarioProperties::prepareScenarioSimulationSettingsFolder()
{
   AddChild(m_scenario_simulation_settings_folder.get());
}

void ScenarioProperties::prepareScenarioMapSettingsFolder()
{
   AddChild(m_scenario_map_settings_folder.get());
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

void ScenarioProperties::OnEnvironmentSettingChanged()
{}

void ScenarioProperties::OnSimulationSettingChanged()
{}

void ScenarioProperties::OnMapSettingChanged()
{}

#undef CR_SCEN_PATH_STT
#undef CR_SCEN_RES_STT
#undef CR_SCEN_ENV_STT
#undef CR_SCEN_SIM_STT
#undef CR_SCEN_MAP_STT