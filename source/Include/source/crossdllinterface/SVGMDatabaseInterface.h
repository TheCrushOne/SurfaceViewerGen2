#pragma once

#include "SurfaceInterfaces.h"
#include "common/database_misc.h"
#include "common/file_storage.h"
#include "common/communicator.h"

namespace database
{
   struct iSVGMDatabaseController : colreg::iReleasable
   {
      virtual void Init(ICommunicator* comm, const file_utils::file_storage_base& dst) = 0;
      virtual void SaveScenarioData(const settings::application_settings& settings, const std::vector<std::vector<double>>& coordGrid) = 0;
      virtual void SaveAppSettings(const settings::application_settings& settings) = 0;
      virtual void LoadScenarioData(settings::application_settings& settings, std::vector<std::vector<double>>& coordGrid) = 0;
      virtual void LoadAppSettings(settings::application_settings& settings) = 0;
   };
}

#ifdef SVGMDATABASECONTROLLER_DLL
#define SVGMDBCEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SVGMDBCEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SVGMDBCEXPRTIMPRT database::iSVGMDatabaseController * CreateSQLiteDatabaseController();
extern "C" SVGMDBCEXPRTIMPRT database::iSVGMDatabaseController * CreateXMLDatabaseController();