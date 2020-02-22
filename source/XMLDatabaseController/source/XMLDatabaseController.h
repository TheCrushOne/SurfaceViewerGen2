#pragma once

#include "crossdllinterface\SVGMDatabaseInterface.h"

namespace database
{
   class XMLDatabaseController : public iSVGMDatabaseController
   {
   public:
      XMLDatabaseController();
      void SaveExternalData(const file_utils::file_storage_base& dst, const double** coordGrid, const chart_meta& meta);

      void Release() override { delete this; }
   };
}