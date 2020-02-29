#pragma once

#include "crossdllinterface\SVGMDatabaseInterface.h"

namespace database
{
   class XMLDatabaseController : public iSVGMDatabaseController
   {
   public:
      XMLDatabaseController();
      void Init(const file_utils::file_storage_base& dst);
      void SaveScenarioData(const settings::application_settings& settings, const double** coordGrid) override final;
      void SaveAppSettings(const settings::application_settings& settings) override final;
      void Release() override { delete this; }
   };
}