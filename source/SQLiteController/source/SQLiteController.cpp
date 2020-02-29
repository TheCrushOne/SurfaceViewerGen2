#include "stdafx.h"
#include "SQLiteController.h"
#include "SqlRequestStorage.h"

// NOTE: запаска
//  ##__VA_ARGS__

#define RESREQLAUNCH(req, ...) sprintf_s(buffer, req.c_str(), ##__VA_ARGS__); m_connector->SQLRequest(buffer, nullptr);

#define NORESREQLAUNCH(req, ...) sprintf_s(buffer, req.c_str(), ##__VA_ARGS__); m_connector->SQLRequest(buffer, nullptr);
#define STTREQLAUNCH(req, param, val) NORESREQLAUNCH(req, param, val)

#define INSUPDSTTREQLAUNCH(reqtoken, param, val) STTREQLAUNCH(request_storage::insupdReqList.at(request_storage::RequestToken::reqtoken), param, val)
#define PTHINSUPDSTTREQLAUNCH(paramtoken, val) INSUPDSTTREQLAUNCH(RT_SETPTHSTT, settings::pathfindingSettingsMeta.at(settings::PathfindingSettingsUID::paramtoken).c_str(), std::to_string(val).c_str())
#define MAPINSUPDSTTREQLAUNCH(paramtoken, val) INSUPDSTTREQLAUNCH(RT_SETMAPSTT, settings::mapSettingsMeta.at(settings::MapSettingsUID::paramtoken).c_str(), std::to_string(val).c_str())

#define SELSTTREQLAUNCH(reqtoken, param, val) STTREQLAUNCH(request_storage::selectReqList.at(request_storage::RequestToken::reqtoken), param, val)

using namespace database;

SQLiteController::SQLiteController()
   : m_connector(std::make_unique<Connector>())
{
   if (!m_shareProvider.IsValid())
   {
      m_shareProvider.Create(SVGUtils::CurrentDllPath("DataShareProvider").c_str(), "CreateDataShareProvider");
      if (!m_shareProvider.IsValid())
      {
         //MessageString(ICommunicator::MS_Error, "Can't load settings serializer!");
         return;
      }
      //return;
   }
}

void SQLiteController::Init(const file_utils::file_storage_base& dst)
{
   m_filestorage = reinterpret_cast<file_utils::sqlite_database_file_storage&>(const_cast<file_utils::file_storage_base&>(dst));
   m_connector->Connect(SVGUtils::wstringToString(m_filestorage.database_path).c_str());
   baseCheckCreate();
}

void SQLiteController::SaveScenarioData(const settings::application_settings& settings, const double** coordGrid)
{
   m_shareProvider->Share(m_filestorage.coordinate_map_path.c_str(), settings.map_stt, coordGrid);
   SaveAppSettings(settings);
}

void SQLiteController::SaveAppSettings(const settings::application_settings& settings)
{
   savePathfindingSettings(settings.pth_stt);
   saveResearchSettings(settings.res_stt);
   saveUnitData(settings.unit_stt);
   saveMapSettings(settings.map_stt);
}

void SQLiteController::LoadScenarioData(settings::application_settings& settings, double*** coordGrid)
{
   LoadAppSettings(settings);
   m_shareProvider->GetShared(m_filestorage.coordinate_map_path.c_str(), settings.map_stt, coordGrid);
}

void SQLiteController::LoadAppSettings(settings::application_settings& settings)
{
   loadMapSettings(settings.map_stt);
}

void SQLiteController::baseCheckCreate()
{
   for (auto& req : request_storage::createReqList)
      m_connector->SQLRequest(req.second.c_str(), nullptr);
}

void SQLiteController::savePathfindingSettings(const settings::pathfinding_settings& settings)
{
   char buffer[512]; buffer[0] = '\0';
   
   PTHINSUPDSTTREQLAUNCH(PSU_MAXAIRHEIGHT, settings.level_settings.max_air_height);
   PTHINSUPDSTTREQLAUNCH(PSU_MAXLANDANGLE, settings.level_settings.max_land_angle);
   PTHINSUPDSTTREQLAUNCH(PSU_DANGEROUSLANDANGLE, settings.level_settings.dangerous_land_angle);
   PTHINSUPDSTTREQLAUNCH(PSU_MAXLANDHEIGHT, settings.level_settings.max_land_height);
   PTHINSUPDSTTREQLAUNCH(PSU_MINLANDHEIGHT, settings.level_settings.min_land_height);
}

void SQLiteController::saveResearchSettings(const settings::research_settings& settings)
{
   //char buffer[512];
   //INSREQLAUNCH(m_connector, buffer, request_storage::settingsReqList.at(request_storage::RequestToken::RT_SETRESSTT), settings::researchSettingsMeta.at(settings::PathfindingSettingsUID::PSU_MINLANDHEIGHT), settings.level_settings.min_land_height);
}

void SQLiteController::saveUnitData(const settings::unit_settings& settings)
{

}

void SQLiteController::saveMapSettings(const settings::map_settings& settings)
{
   char buffer[512]; buffer[0] = '\0';

   MAPINSUPDSTTREQLAUNCH(MSU_ROWCOUNT, settings.row_count);
   MAPINSUPDSTTREQLAUNCH(MSU_COLCOUNT, settings.col_count);
}

void SQLiteController::loadPathfindingSettings(settings::pathfinding_settings& settings)
{

}

void SQLiteController::loadResearchSettings(settings::research_settings& settings)
{

}

void SQLiteController::loadUnitData(settings::unit_settings& settings)
{

}

void SQLiteController::loadMapSettings(settings::map_settings& settings)
{
   char buffer[512]; buffer[0] = '\0';

   auto callback = [](void* param, int argc, char** argv, char** azColName)->int
   {
      return 1;
   };

   sprintf_s(buffer, request_storage::insupdReqList.at(request_storage::RequestToken::RT_SELMAPSTT).c_str(), settings::mapSettingsMeta.at(settings::MapSettingsUID::MSU_ROWCOUNT).c_str());
   m_connector->SQLRequest(buffer, callback);
}

database::iSVGMDatabaseController* CreateSQLiteDatabaseController()
{
   return new database::SQLiteController();
}