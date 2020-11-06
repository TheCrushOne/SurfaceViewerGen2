#pragma once
#include "SelectedObjectManager.h"
#include "properties/property_helper.h"

namespace SV
{
   struct debug_filter_info
   {
      std::string name;
      bool show = false;
   };

   class ScenarioProperties : public iSelected
   {
      enum ScenarioPathfindingSettingsFieldIndex
      {
         // level settings
         SPSFI_MAXAIRHEIGHT = 0,
         SPSFI_MAXLANDHEIGHT,
         SPSFI_MINLANDHEIGHT,
         SPSFI_MAXLANDANGLE,
         SPSFI_DANGEROUSLANDANGLE,

         SPSFI_END,
      };

      enum ScenarioResearchSettingsFieldIndex
      {
         // count range
         SRSFI_COUTNRANGE_MIN = 0,
         SRSFI_COUTNRANGE_MAX,
         SRSFI_COUTNRANGE_STEP,

         // length range
         SRSFI_LEGNTHRANGE_MIN,
         SRSFI_LEGNTHRANGE_MAX,
         SRSFI_LEGNTHRANGE_STEP,

         SRSFI_ITERCOUNT,
         SRSFI_MAPSIZE,
         SRSFI_MULTITHREADTEST,
         SRSFI_SINGLETHREADTEST,
         SRSFI_DEBUGLEVEL,

         SRSFI_END,
      };

      enum ScenarioEnvironmentSettingsFieldIndex
      {
         // geographical coordinate system
         SESFI_GCS_ANGLE = 0,
         SESFI_GCS_SCALE,
         SESFI_GCS_ORD_BIAS,
         SESFI_GCS_ABS_BIAS,

         // matrix coordinate system
         SESFI_MTX_ANGLE,
         SESFI_MTX_SCALE,
         SESFI_MTX_ORD_BIAS,
         SESFI_MTX_ABS_BIAS,

         SESFI_END,
      };

      enum ScenarioSimulationSettingsFieldIndex
      {
         SSSFI_END = 0,
      };

      enum ScenarioMapSettingsFieldIndex
      {
         SMSFI_ROW_COUNT,
         SMSFI_COL_COUNT,

         SMSFI_END,
      };

      const std::unordered_map<ScenarioPathfindingSettingsFieldIndex, FieldMeta> scenarioPathfindingSettingsMeta = {
         { SPSFI_MAXAIRHEIGHT, { "Max air height", "Maximum air height for drones", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SPSFI_MAXLANDHEIGHT, { "Max land height", "Maximum land height for rovers", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SPSFI_MINLANDHEIGHT, { "Min land height", "Minimum land height for rovers", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SPSFI_MAXLANDANGLE, { "Max land angle", "Maximum land angle for rovers", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SPSFI_DANGEROUSLANDANGLE, { "Dangerous land angle", "Dangerous land angle for rovers", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      };

      const std::vector<std::string> debugModes = {
         { "Single Only" },
         { "Multi Only" },
         { "Full" },
      };

      const std::unordered_map<ScenarioResearchSettingsFieldIndex, FieldMeta> scenarioResearchSettingsMeta = {
         { SRSFI_COUTNRANGE_MIN, { "Min", "minimal value", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SRSFI_COUTNRANGE_MAX, { "Max", "maximum value", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SRSFI_COUTNRANGE_STEP, { "Step", "step value", VALUE_FORMAT_TYPE::VFT_NONE, false } },

         { SRSFI_LEGNTHRANGE_MIN, { "Min", "minimal value", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SRSFI_LEGNTHRANGE_MAX, { "Max", "maximum value", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SRSFI_LEGNTHRANGE_STEP, { "Step", "step value", VALUE_FORMAT_TYPE::VFT_NONE, false } },

         { SRSFI_ITERCOUNT, { "obstacleDistSafe", "safe distance to obstacle", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SRSFI_MAPSIZE, { "debugNarrow", "for swimming in narrows(colreg check)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SRSFI_MULTITHREADTEST, { "debugNarrowWidth", "narrow width, miles", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SRSFI_SINGLETHREADTEST, { "analysisObjectsMask", "defines which objects will be used while analysing", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SRSFI_DEBUGLEVEL, { "analysisObjectsMask", "defines which objects will be used while analysing", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      };

      const std::unordered_map<ScenarioEnvironmentSettingsFieldIndex, FieldMeta> scenarioEnvironmentSettingsMeta = {
         { SESFI_GCS_ANGLE, { "Angle", "Grid relative rotation angle", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SESFI_GCS_SCALE, { "Scale", "Coordinate grid scale", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SESFI_GCS_ORD_BIAS, { "Ordinate bias", "Ordinate bias", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SESFI_GCS_ABS_BIAS, { "Abscissa bias", "Abscissa bias", VALUE_FORMAT_TYPE::VFT_NONE, false } },

         { SESFI_MTX_ANGLE, { "Angle", "Grid relative rotation angle", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SESFI_MTX_SCALE, { "Scale", "Coordinate grid scale", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SESFI_MTX_ORD_BIAS, { "Ordinate bias", "Ordinate bias", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SESFI_MTX_ABS_BIAS, { "Abscissa bias", "Abscissa bias", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      };

      const std::unordered_map<ScenarioSimulationSettingsFieldIndex, FieldMeta> scenarioSimulationSettingsMeta = {
      };

      const std::unordered_map<ScenarioMapSettingsFieldIndex, FieldMeta> scenarioMapSettingsMeta = {
         { SMSFI_ROW_COUNT, { "Row", "Map row count", VALUE_FORMAT_TYPE::VFT_NONE, false } },
         { SMSFI_COL_COUNT, { "Col", "Map col count", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      };
   public:
      ScenarioProperties();
      void Render(render::iRender* renderer) override {}

      bool IsUpdateable() const override final { return true; }
   private:
      void OnPathfindingSettingChanged();
      void OnResearchSettingChanged();
      void OnEnvironmentSettingChanged();
      void OnSimulationSettingChanged();
      void OnMapSettingChanged();

      void prepareScenarioPathfindingSettingsFolder();
      void prepareScenarioResearchSettingsFolder();
      void prepareScenarioEnvironmentSettingsFolder();
      void prepareScenarioSimulationSettingsFolder();
      void prepareScenarioMapSettingsFolder();

      //void addDebugFilter(const dbg::debug_info_node& node, FolderProperty* folder);
   private:
      // pathfinding
      std::unique_ptr<FolderProperty> m_scenario_pathfinding_settings_folder;

      std::unique_ptr<FolderProperty> m_scenario_pathfinding_settings_level_folder;

      // research
      std::unique_ptr<FolderProperty> m_scenario_research_settings_folder;
      std::unique_ptr<FolderProperty> m_scenario_research_settings_countrange_folder;
      std::unique_ptr<FolderProperty> m_scenario_research_settings_lengthrange_folder;

      // environment
      std::unique_ptr<FolderProperty> m_scenario_environment_settings_folder;
      std::unique_ptr<FolderProperty> m_scenario_environment_settings_gcs_folder;
      std::unique_ptr<FolderProperty> m_scenario_environment_settings_mtx_folder;

      // simulation
      std::unique_ptr<FolderProperty> m_scenario_simulation_settings_folder;

      // map
      std::unique_ptr<FolderProperty> m_scenario_map_settings_folder;

      //settings properties
      std::vector<iPropertyPtr> m_prop_pathfinding_settings;
      std::vector<iPropertyPtr> m_prop_research_settings;
      std::vector<iPropertyPtr> m_prop_environment_settings;
      std::vector<iPropertyPtr> m_prop_simulation_settings;
      std::vector<iPropertyPtr> m_prop_map_settings;

      settings::application_settings m_settings;
   };
}