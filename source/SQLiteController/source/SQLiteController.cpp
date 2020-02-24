#include "stdafx.h"
#include "SQLiteController.h"
#include "SqlRequestStorage.h"

#define INSREQLAUNCH(con, buf, req, ...) sprintf_s(buf, req.c_str(), ##__VA_ARGS__); con->SQLRequest(buf, nullptr);

using namespace database;

SQLiteController::SQLiteController()
   : m_connector(std::make_unique<Connector>())
{}

void SQLiteController::SaveHeightGrid(const file_utils::file_storage_base& dst, const double** coordGrid, const chart_meta& meta)
{
   auto& destFs = reinterpret_cast<file_utils::sqlite_database_file_storage&>(const_cast<file_utils::file_storage_base&>(dst));
   m_connector->Connect(SVGUtils::wstringToString(destFs.database_path).c_str());
}

void SQLiteController::SaveAppSettings(const settings::application_settings& settings)
{
   savePathfindingSettings(settings.pth_stt);
   saveResearchSettings(settings.res_stt);
   saveUnitData(settings.unit_stt);
}

void SQLiteController::baseCheckCreate()
{
   for (auto& req : request_storage::createReqList)
      m_connector->SQLRequest(req.second.c_str(), nullptr);
}

void SQLiteController::savePathfindingSettings(const settings::pathfinding_settings& settings)
{
   //settings.level_settings.dangerous_land_angle
   char buffer[512]; buffer[0] = '\0';
   //const char* command = request_storage::settingsReqList.at(request_storage::RequestToken::RT_SETPTHSTT).c_str();
   //std::string param = settings::pathfindingSettingsMeta.at(settings::PathfindingSettingsUID::PSU_MAXAIRHEIGHT);
   //std::string value = std::to_string(settings.level_settings.max_air_height);
   //sprintf_s(buffer, 512, command, param.c_str(), value.c_str()); m_connector->SQLRequest(buffer, nullptr);
   INSREQLAUNCH(m_connector, buffer, request_storage::settingsReqList.at(request_storage::RequestToken::RT_SETPTHSTT), settings::pathfindingSettingsMeta.at(settings::PathfindingSettingsUID::PSU_MAXAIRHEIGHT).c_str(), std::to_string(settings.level_settings.max_air_height).c_str());
   INSREQLAUNCH(m_connector, buffer, request_storage::settingsReqList.at(request_storage::RequestToken::RT_SETPTHSTT), settings::pathfindingSettingsMeta.at(settings::PathfindingSettingsUID::PSU_MAXLANDANGLE).c_str(), std::to_string(settings.level_settings.max_land_angle).c_str());
   INSREQLAUNCH(m_connector, buffer, request_storage::settingsReqList.at(request_storage::RequestToken::RT_SETPTHSTT), settings::pathfindingSettingsMeta.at(settings::PathfindingSettingsUID::PSU_DANGEROUSLANDANGLE).c_str(), std::to_string(settings.level_settings.dangerous_land_angle).c_str());
   INSREQLAUNCH(m_connector, buffer, request_storage::settingsReqList.at(request_storage::RequestToken::RT_SETPTHSTT), settings::pathfindingSettingsMeta.at(settings::PathfindingSettingsUID::PSU_MAXLANDHEIGHT).c_str(), std::to_string(settings.level_settings.max_land_height).c_str());
   INSREQLAUNCH(m_connector, buffer, request_storage::settingsReqList.at(request_storage::RequestToken::RT_SETPTHSTT), settings::pathfindingSettingsMeta.at(settings::PathfindingSettingsUID::PSU_MINLANDHEIGHT).c_str(), std::to_string(settings.level_settings.min_land_height).c_str());
}

void SQLiteController::saveResearchSettings(const settings::research_settings& settings)
{
   //char buffer[512];
   //INSREQLAUNCH(m_connector, buffer, request_storage::settingsReqList.at(request_storage::RequestToken::RT_SETRESSTT), settings::researchSettingsMeta.at(settings::PathfindingSettingsUID::PSU_MINLANDHEIGHT), settings.level_settings.min_land_height);
}

void SQLiteController::saveUnitData(const settings::unit_settings& settings)
{

}

database::iSVGMDatabaseController* CreateSQLiteDatabaseController()
{
   return new database::SQLiteController();
}