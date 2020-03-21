#include "stdafx.h"
#include "SQLiteController.h"
#include "SqlRequestStorage.h"
#include "database/sqlite3.h"

// NOTE: запаска
//  ##__VA_ARGS__

#define NO_RES_PREP char buffer[65536]; buffer[0] = '\0';
#define RES_PREP_INT int rvalbuf; NO_RES_PREP
#define RES_PREP_STR std::string rvalbuf; NO_RES_PREP

#define RES_INT_RL(req, param, val) sprintf_s(buffer, req.c_str(), param); m_connector->SQLResRequest(buffer, [&rvalbuf](sqlite3_stmt* stmt) \
   { \
      std::string valbuf(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))); \
      if (valbuf.compare(param) == 0) \
      { \
         rvalbuf = sqlite3_column_int(stmt, 1); \
      } \
      return 0; \
   } \
); val = rvalbuf;

#define RES_STR_RL(req, param, val) sprintf_s(buffer, req.c_str(), param); m_connector->SQLResRequest(buffer, [&rvalbuf](sqlite3_stmt* stmt) \
   { \
      std::string valbuf(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))); \
      if (valbuf.compare(param) == 0) \
      { \
         rvalbuf = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)); \
      } \
      return 0; \
   } \
); val = rvalbuf;

#define STT_RES_INT_RL(req, param, val) RES_INT_RL(req, param, val)
#define STT_RES_STR_RL(req, param, val) RES_STR_RL(req, param, val)
#define DAT_RES_STR_RL(req, param, val) RES_STR_RL(req, param, val)

#define NORES_RL(req, ...) sprintf_s(buffer, req.c_str(), ##__VA_ARGS__); m_connector->SQLNoResRequest(buffer);
#define STT_NORES_RL(req, param, val) NORES_RL(req, param, val)

#define INSUPD_STT_NORES_RL(reqtoken, param, val) STT_NORES_RL(request_storage::insupdReqList.at(request_storage::RequestToken::reqtoken), param, val)
#define PTH_INSUPD_STT_NORES_RL(paramtoken, val) INSUPD_STT_NORES_RL(RT_SETPTHSTT, settings::pathfindingSettingsMeta.at(settings::PathfindingSettingsUID::paramtoken).c_str(), std::to_string(val).c_str())
#define RES_INSUPD_STT_NORES_RL(paramtoken, val) INSUPD_STT_NORES_RL(RT_SETRESSTT, settings::researchSettingsMeta.at(settings::ResearchSettingsUID::paramtoken).c_str(), std::to_string(val).c_str())
#define MAP_INSUPD_STT_NORES_RL(paramtoken, val) INSUPD_STT_NORES_RL(RT_SETMAPSTT, settings::mapSettingsMeta.at(settings::MapSettingsUID::paramtoken).c_str(), std::to_string(val).c_str())
#define UNT_INSUPD_NORES_RL(json) NORES_RL(request_storage::insupdReqList.at(request_storage::RequestToken::RT_SETUNTDAT), json)
#define STT_INS_NORES_RL(token, json) NORES_RL(request_storage::insReqList.at(request_storage::RequestToken::RT_SETSCNSTT), request_storage::settingsTokenList.at(token).c_str(), json)

#define SEL_STT_RES_INT_RL(reqtoken, param, val) STT_RES_INT_RL(request_storage::selectReqList.at(request_storage::RequestToken::reqtoken), param, val)
#define MAP_SEL_STT_RES_INT_RL(paramtoken, val) SEL_STT_RES_INT_RL(RT_SELMAPSTT, settings::mapSettingsMeta.at(settings::MapSettingsUID::paramtoken).c_str(), val)

// HACK: пока настройки json-овые, там лежит всего одна запись; её и берем
#define UNT_SEL_DAT_RES_STR_RL(val) DAT_RES_STR_RL(request_storage::selectReqList.at(request_storage::RequestToken::RT_SELUNTDAT), "1", val)

#define STT_SEL_DAT_RES_STR_RL(token, val) DAT_RES_STR_RL(request_storage::selectReqList.at(request_storage::RequestToken::RT_SELSCNSTT), request_storage::settingsTokenList.at(token).c_str(), val)
using namespace database;

SQLiteController::SQLiteController()
   : Communicable(nullptr)
{}

void SQLiteController::Init(ICommunicator* comm, const file_utils::global_path_storage& dst)
{
   m_communicator = comm;
   m_connector = std::make_unique<Connector>(comm);
   m_shareProvider.Create(SVGUtils::CurrentDllPath("DataShareProvider").c_str(), "CreateDataShareProvider");
   if (!m_shareProvider.IsValid())
   {
      m_communicator->Message(ICommunicator::MS_Error, "Can't load 'DataShareProvider'!");
      return;
   }
   //return;

   m_unitDataSerializer.Create(SVGUtils::CurrentDllPath("SettingsHandler").c_str(), "CreateUnitDataSerializer");
   if (!m_unitDataSerializer.IsValid())
   {
      m_communicator->Message(ICommunicator::MS_Error, "Can't load 'SettingsHandler'!");
      return;
   }

   m_settingsSerializer.Create(SVGUtils::CurrentDllPath("SettingsHandler").c_str(), "CreateJsonSettingsSerializer");
   if (!m_settingsSerializer.IsValid())
   {
      m_communicator->Message(ICommunicator::MS_Error, "Can't load 'SettingsHandler'!");
      return;
   }
   m_filestorage = const_cast<file_utils::global_path_storage&>(dst);
   m_connector->Connect(SVGUtils::wstringToString(m_filestorage.database_path).c_str());
   baseCheckCreate();
}

void SQLiteController::SaveScenarioData(const settings::application_settings& settings, const settings::unit_source_data& unitData, const std::vector<std::vector<double>>& coordGrid)
{
   m_shareProvider->Share(m_filestorage.coordinate_map_path.c_str(), settings.map_stt, coordGrid);
   SaveAppSettings(settings);
   saveUnitData(unitData);
}

void SQLiteController::SaveAppSettings(const settings::application_settings& settings)
{
   savePathfindingSettings(settings.pth_stt);
   saveResearchSettings(settings.res_stt);
   saveEnvironmentSettings(settings.env_stt);
   saveSimulationSettings(settings.sim_stt);
   saveMapSettings(settings.map_stt);
}

void SQLiteController::LoadScenarioData(settings::application_settings& settings, settings::unit_source_data& unitData, std::vector<std::vector<double>>& coordGrid)
{
   LoadAppSettings(settings);
   loadUnitData(unitData);
   m_shareProvider->GetShared(m_filestorage.coordinate_map_path.c_str(), settings.map_stt, coordGrid);
}

void SQLiteController::LoadAppSettings(settings::application_settings& settings)
{
   loadMapSettings(settings.map_stt);
   loadSimulationSettings(settings.sim_stt);
   loadEnvironmentSettings(settings.env_stt);
   loadResearchSettings(settings.res_stt);
   loadPathfindingSettings(settings.pth_stt);
}

void SQLiteController::baseCheckCreate()
{
   for (auto& req : request_storage::createReqList)
      m_connector->SQLNoResRequest(req.second.c_str());
}

void SQLiteController::savePathfindingSettings(const settings::pathfinding_settings& settings)
{
   NO_RES_PREP;
   std::string jsonSettings = m_settingsSerializer->ToString(settings);

   STT_INS_NORES_RL(request_storage::SettingsType::ST_PATHFINDING, jsonSettings.c_str()); 
   //sprintf_s(buffer, request_storage::insReqList.at(request_storage::RequestToken::RT_SETSCNSTT).c_str(), request_storage::settingsTokenList.at(request_storage::SettingsType::ST_PATHFINDING).c_str(), jsonSettings.c_str());
   //m_connector->SQLNoResRequest(buffer);
}

void SQLiteController::saveResearchSettings(const settings::research_settings& settings)
{
   NO_RES_PREP;
   std::string jsonSettings = m_settingsSerializer->ToString(settings);

   STT_INS_NORES_RL(request_storage::SettingsType::ST_RESEARCH, jsonSettings.c_str());
}

void SQLiteController::saveEnvironmentSettings(const settings::environment_settings& settings)
{
   NO_RES_PREP;
   std::string jsonSettings = m_settingsSerializer->ToString(settings);

   STT_INS_NORES_RL(request_storage::SettingsType::ST_ENVIRONMENT, jsonSettings.c_str());
}

void SQLiteController::saveSimulationSettings(const settings::simulation_settings& settings)
{
   NO_RES_PREP;
   std::string jsonSettings = m_settingsSerializer->ToString(settings);

   STT_INS_NORES_RL(request_storage::SettingsType::ST_SIMULATION, jsonSettings.c_str());
}

void SQLiteController::saveMapSettings(const settings::map_settings& settings)
{
   NO_RES_PREP;
   std::string jsonSettings = m_settingsSerializer->ToString(settings);

   STT_INS_NORES_RL(request_storage::SettingsType::ST_MAP, jsonSettings.c_str());
}

void SQLiteController::saveUnitData(const settings::unit_source_data& settings)
{
   NO_RES_PREP;
   std::string jsonSettings = m_unitDataSerializer->ToString(settings);

   //sprintf_s(buffer, request_storage::insupdReqList.at(request_storage::RequestToken::RT_SETUNTDAT).c_str(), jsonSettings.c_str()); m_connector->SQLNoResRequest(buffer);
   UNT_INSUPD_NORES_RL(jsonSettings.c_str());
}

void SQLiteController::loadPathfindingSettings(settings::pathfinding_settings& settings)
{
   RES_PREP_STR;
   std::string dat;

   STT_SEL_DAT_RES_STR_RL(request_storage::SettingsType::ST_PATHFINDING, dat);

   m_settingsSerializer->FromString(dat.c_str(), settings);
}

void SQLiteController::loadResearchSettings(settings::research_settings& settings)
{
   RES_PREP_STR;
   std::string dat;

   STT_SEL_DAT_RES_STR_RL(request_storage::SettingsType::ST_RESEARCH, dat);

   m_settingsSerializer->FromString(dat.c_str(), settings);
}

void SQLiteController::loadEnvironmentSettings(settings::environment_settings& settings)
{
   RES_PREP_STR;
   std::string dat;

   STT_SEL_DAT_RES_STR_RL(request_storage::SettingsType::ST_ENVIRONMENT, dat);

   m_settingsSerializer->FromString(dat.c_str(), settings);
}

void SQLiteController::loadSimulationSettings(settings::simulation_settings& settings)
{
   RES_PREP_STR;
   std::string dat;

   STT_SEL_DAT_RES_STR_RL(request_storage::SettingsType::ST_SIMULATION, dat);

   m_settingsSerializer->FromString(dat.c_str(), settings);
}

void SQLiteController::loadMapSettings(settings::map_settings& settings)
{
   RES_PREP_STR;
   std::string dat;

   STT_SEL_DAT_RES_STR_RL(request_storage::SettingsType::ST_MAP, dat);

   m_settingsSerializer->FromString(dat.c_str(), settings);
}

void SQLiteController::loadUnitData(settings::unit_source_data& settings)
{
   RES_PREP_STR;
   std::string dat;

   UNT_SEL_DAT_RES_STR_RL(dat);

   m_unitDataSerializer->FromString(dat.c_str(), settings);
}

database::iSVGMDatabaseController* CreateSQLiteDatabaseController()
{
   return new database::SQLiteController();
}