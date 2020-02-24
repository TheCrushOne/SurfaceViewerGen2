#pragma once

#include "crossdllinterface\SVGMDatabaseInterface.h"

namespace database
{
   class XMLDatabaseController : public iSVGMDatabaseController
   {
   public:
      XMLDatabaseController();
      void SaveHeightGrid(const file_utils::file_storage_base& dst, const double** coordGrid, const chart_meta& meta) override final;
      void SaveAppSettings(const settings::application_settings& settings) override final;
      void Release() override { delete this; }
   };
}