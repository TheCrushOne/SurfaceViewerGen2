#pragma once

#include "crossdllinterface\SVGMDatabaseInterface.h"
#include "crossdllinterface/DataShareInterface.h"
#include "crossdllinterface\UnitDataSerializerInterface.h"
#include "crossdllinterface\SettingsSerializerInterface.h"
#include "Connector.h"
#include "colreg/ModuleGuard.h"

namespace SV::database
{
   class SQLiteController : public iSVGMDatabaseController, public Central
   {
   public:
      SQLiteController(central_pack* pack);
      void Connect(const char*) override final;
      void SaveScenarioData(const settings::unit_source_data& unitData, const std::vector<std::vector<double>>& coordGrid) override final;
      void SaveAppSettings() override final;
      void SaveDataStandartHashJunction(data_hash::hash_junction&) override final;
      void LoadScenarioData(settings::unit_source_data& unitData, std::vector<std::vector<double>>& coordGrid) override final;
      void LoadAppSettings() override final;
      bool CheckDataStandartHashJunction(data_hash::hash_junction&) override final;
      void Release() override { m_connector->Disconnect(); delete this; }
   private:
      void baseCheckCreate();

      void savePathfindingSettings();
      void saveResearchSettings();
      void saveEnvironmentSettings();
      void saveSimulationSettings();
      void saveMapSettings();
      void saveUnitData(const settings::unit_source_data& settings);

      void loadPathfindingSettings();
      void loadResearchSettings();
      void loadEnvironmentSettings();
      void loadSimulationSettings();
      void loadMapSettings();
      void loadUnitData(settings::unit_source_data& settings);
   private:
      std::unique_ptr<Connector> m_connector;
      system::ModuleGuard<data_share::iDataShareProvider, central_pack*> m_shareProvider;
      system::ModuleGuard<serializer::iUnitDataSerializer> m_unitDataSerializer;
      system::ModuleGuard<serializer::iSettingsSerializer> m_settingsSerializer;
   };
}