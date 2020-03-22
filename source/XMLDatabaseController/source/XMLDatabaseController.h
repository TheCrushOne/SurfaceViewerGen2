#pragma once

#include "crossdllinterface\SVGMDatabaseInterface.h"

namespace database
{
   class XMLDatabaseController : public iSVGMDatabaseController
   {
   public:
      XMLDatabaseController();
      void Init(ICommunicator* comm) override final;
      void SaveScenarioData(const settings::unit_source_data& unitData, const std::vector<std::vector<double>>& coordGrid) override final;
      void SaveAppSettings() override final;
      void LoadScenarioData(settings::unit_source_data& unitData, std::vector<std::vector<double>>& coordGrid) override final;
      void LoadAppSettings() override final;
      void Release() override { delete this; }
   };
}