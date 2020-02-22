#pragma once

#include "SurfaceInterfaces.h"
#include "common/database_misc.h"
#include "common/file_storage.h"

namespace database
{
   struct iSVGMDatabaseController : colreg::iReleasable
   {
      virtual void SaveExternalData(const file_utils::file_storage_base& dst, const double** coordGrid, const chart_meta& meta) = 0;
   };
}

extern "C" EXPRTIMPRT database::iSVGMDatabaseController * CreateSQLiteDatabaseController();
extern "C" EXPRTIMPRT database::iSVGMDatabaseController * CreateXMLDatabaseController();