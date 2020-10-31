#pragma once

#include "common/file_storage.h"
#include "common/communicator.h"
#include "common/central_class.h"
#include "common/data_hash.h"
#include "colreg/BaseRef.h"

namespace SV::database
{
   struct iSVGMDatabaseController : iReleasable
   {
      virtual void Connect(const char*) = 0;
      virtual void SaveScenarioData(const settings::unit_source_data&, const std::vector<std::vector<double>>&) = 0;
      virtual void SaveAppSettings() = 0;
      virtual void SaveDataStandartHashJunction(data_hash::hash_junction&) = 0;
      virtual void LoadScenarioData(settings::unit_source_data&, std::vector<std::vector<double>>&) = 0;
      virtual void LoadAppSettings() = 0;
      virtual bool CheckDataStandartHashJunction(data_hash::hash_junction&) = 0;
   };
}

#ifdef SVGMDATABASECONTROLLER_DLL
#define SVGMDBCEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SVGMDBCEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SVGMDBCEXPRTIMPRT SV::database::iSVGMDatabaseController * CreateSQLiteDatabaseController(SV::central_pack*);
extern "C" SVGMDBCEXPRTIMPRT SV::database::iSVGMDatabaseController * CreateXMLDatabaseController(SV::central_pack*);