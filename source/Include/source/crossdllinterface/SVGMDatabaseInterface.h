#pragma once

#include "SurfaceInterfaces.h"
#include "common/database_misc.h"
#include "common/file_storage.h"
#include "common/communicator.h"
#include "common/central_class.h"

namespace database
{
   struct iSVGMDatabaseController : colreg::iReleasable
   {
      virtual void SaveScenarioData(const settings::unit_source_data&, const std::vector<std::vector<double>>&) = 0;
      virtual void SaveAppSettings() = 0;
      virtual void LoadScenarioData(settings::unit_source_data&, std::vector<std::vector<double>>&) = 0;
      virtual void LoadAppSettings() = 0;
   };
}

#ifdef SVGMDATABASECONTROLLER_DLL
#define SVGMDBCEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SVGMDBCEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SVGMDBCEXPRTIMPRT database::iSVGMDatabaseController * CreateSQLiteDatabaseController(central_pack*);
extern "C" SVGMDBCEXPRTIMPRT database::iSVGMDatabaseController * CreateXMLDatabaseController(central_pack*);