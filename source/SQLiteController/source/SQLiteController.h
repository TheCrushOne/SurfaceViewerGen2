#pragma once

#include "crossdllinterface\SVGMDatabaseInterface.h"
#include "crossdllinterface/DataShareInterface.h"
#include "Connector.h"
#include "colreg/ModuleGuard.h"

namespace database
{
   class SQLiteController : public iSVGMDatabaseController
   {
   public:
      SQLiteController();
      void Init(const file_utils::file_storage_base& dst) override final;
      void SaveScenarioData(const settings::application_settings& settings, const double** coordGrid) override final;
      void SaveAppSettings(const settings::application_settings& settings) override final;
      void LoadScenarioData(settings::application_settings& settings, double*** coordGrid) override final;
      void LoadAppSettings(settings::application_settings& settings) override final;
      void Release() override { delete this; }
   private:
      void baseCheckCreate();

      void savePathfindingSettings(const settings::pathfinding_settings& settings);
      void saveResearchSettings(const settings::research_settings& settings);
      void saveUnitData(const settings::unit_settings& settings);
      void saveMapSettings(const settings::map_settings& settings);

      void loadPathfindingSettings(settings::pathfinding_settings& settings);
      void loadResearchSettings(settings::research_settings& settings);
      void loadUnitData(settings::unit_settings& settings);
      void loadMapSettings(settings::map_settings& settings);
   private:
      std::unique_ptr<Connector> m_connector;
      file_utils::sqlite_database_file_storage m_filestorage;
      colreg::ModuleGuard<data_share::iDataShareProvider> m_shareProvider;
   };
}