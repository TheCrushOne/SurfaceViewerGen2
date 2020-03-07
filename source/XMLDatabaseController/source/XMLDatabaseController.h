#pragma once

#include "crossdllinterface\SVGMDatabaseInterface.h"

namespace database
{
   class XMLDatabaseController : public iSVGMDatabaseController
   {
   public:
      XMLDatabaseController();
      void Init(ICommunicator* comm, const file_utils::file_storage_base& dst) override final;
      void SaveScenarioData(const settings::application_settings& settings, const settings::unit_source_data& unitData, const std::vector<std::vector<double>>& coordGrid) override final;
      void SaveAppSettings(const settings::application_settings& settings) override final;
      void LoadScenarioData(settings::application_settings& settings, settings::unit_source_data& unitData, std::vector<std::vector<double>>& coordGrid) override final;
      void LoadAppSettings(settings::application_settings& settings) override final;
      void Release() override { delete this; }
   };
}