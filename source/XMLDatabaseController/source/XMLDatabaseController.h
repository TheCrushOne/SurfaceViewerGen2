#pragma once

#include "crossdllinterface\SVGMDatabaseInterface.h"

namespace SV::database
{
   class XMLDatabaseController : public iSVGMDatabaseController, public Central
   {
   public:
      XMLDatabaseController(central_pack* pack);
      void Connect(const char*) override final;
      void SaveScenarioData(const settings::unit_source_data& unitData, const std::vector<std::vector<double>>& coordGrid) override final;
      void SaveAppSettings() override final;
      void SaveDataStandartHashJunction(data_hash::hash_junction&) override final;
      void LoadScenarioData(settings::unit_source_data& unitData, std::vector<std::vector<double>>& coordGrid) override final;
      void LoadAppSettings() override final;
      bool CheckDataStandartHashJunction(data_hash::hash_junction&) override final;
      void Release() override { delete this; }
   };
}