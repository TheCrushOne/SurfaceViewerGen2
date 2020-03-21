#pragma once

#include "crossdllinterface\SVGMDatabaseInterface.h"
#include "crossdllinterface/DataShareInterface.h"
#include "crossdllinterface\UnitDataSerializerInterface.h"
#include "crossdllinterface\SettingsSerializerInterface.h"
#include "Connector.h"
#include "colreg/ModuleGuard.h"

namespace database
{
   class SQLiteController : public iSVGMDatabaseController, public Communicable
   {
   public:
      SQLiteController();
      void Init(ICommunicator* comm, const file_utils::global_path_storage& dst) override final;
      void SaveScenarioData(const settings::application_settings& settings, const settings::unit_source_data& unitData, const std::vector<std::vector<double>>& coordGrid) override final;
      void SaveAppSettings(const settings::application_settings& settings) override final;
      void LoadScenarioData(settings::application_settings& settings, settings::unit_source_data& unitData, std::vector<std::vector<double>>& coordGrid) override final;
      void LoadAppSettings(settings::application_settings& settings) override final;
      void Release() override { delete this; }
   private:
      void baseCheckCreate();

      void savePathfindingSettings(const settings::pathfinding_settings& settings);
      void saveResearchSettings(const settings::research_settings& settings);
      void saveEnvironmentSettings(const settings::environment_settings& settings);
      void saveSimulationSettings(const settings::simulation_settings& settings);
      void saveMapSettings(const settings::map_settings& settings);
      void saveUnitData(const settings::unit_source_data& settings);

      void loadPathfindingSettings(settings::pathfinding_settings& settings);
      void loadResearchSettings(settings::research_settings& settings);
      void loadEnvironmentSettings(settings::environment_settings& settings);
      void loadSimulationSettings(settings::simulation_settings& settings);
      void loadMapSettings(settings::map_settings& settings);
      void loadUnitData(settings::unit_source_data& settings);
   private:
      std::unique_ptr<Connector> m_connector;
      file_utils::global_path_storage m_filestorage;
      colreg::ModuleGuard<data_share::iDataShareProvider> m_shareProvider;
      colreg::ModuleGuard<colreg::iUnitDataSerializer> m_unitDataSerializer;
      colreg::ModuleGuard<colreg::iSettingsSerializer> m_settingsSerializer;
   };
}