#pragma once

#include "crossdllinterface\SVGMDatabaseInterface.h"
#include "Connector.h"

namespace database
{
   class SQLiteController : public iSVGMDatabaseController
   {
   public:
      SQLiteController();
      void SaveHeightGrid(const file_utils::file_storage_base& dst, const double** coordGrid, const chart_meta& meta) override final;
      void SaveAppSettings(const settings::application_settings& settings) override final;
      void Release() override { delete this; }
   private:
      void baseCheckCreate();

      void savePathfindingSettings(const settings::pathfinding_settings& settings);
      void saveResearchSettings(const settings::research_settings& settings);
      void saveUnitData(const settings::unit_settings& settings);
   private:
      std::unique_ptr<Connector> m_connector;
   };
}