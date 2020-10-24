#pragma once
#include "SelectedObjectManager.h"
#include "property_helper.h"

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
   public:
      ScenarioProperties();
      void Render(render::iRender* renderer) override {}
   private:
      void OnPathfindingSettingChanged();
      void OnResearchSettingChanged();
      void prepareScenarioPathfindingSettingsFolder();
      void prepareScenarioResearchSettingsFolder();

      //void addDebugFilter(const dbg::debug_info_node& node, FolderProperty* folder);
   private:
      std::unique_ptr<FolderProperty> m_scenario_pathfinding_settings_folder;

      std::unique_ptr<FolderProperty> m_scenario_pathfinding_settings_level_folder;

      std::unique_ptr<FolderProperty> m_scenario_research_settings_folder;
      std::unique_ptr<FolderProperty> m_scenario_research_settings_countrange_folder;
      std::unique_ptr<FolderProperty> m_scenario_research_settings_lengthrange_folder;

      //settings properties
      std::vector<iPropertyPtr> m_prop_pathfinding_settings;
      std::vector<iPropertyPtr> m_prop_research_settings;

      settings::application_settings m_settings;
   };
}