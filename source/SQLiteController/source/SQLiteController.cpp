#include "stdafx.h"
#include "SQLiteController.h"
#include "SqlRequestStorage.h"
#include "database/sqlite3.h"

// NOTE: запаска
//  ##__VA_ARGS__

#define NO_RES_PREP char buffer[10000]; buffer[0] = '\0';
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

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard, ...) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName, __VA_ARGS__); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
      return; \
   }

template<typename... Ts>
void launchNoResultRequest(Connector * conn, const std::string& req, Ts... args)
{
   NO_RES_PREP
   sprintf_s(buffer, req.c_str(), args...);
   conn->SQLNoResRequest(buffer);
}

template<typename... Ts>
bool launchResultExistenceRequest(Connector* conn, const std::string& req, Ts... args)
{
   NO_RES_PREP
   sprintf_s(buffer, req.c_str(), args...);
   bool exists = false;
   conn->SQLResRequest(buffer, [&exists](sqlite3_stmt* stmt) { exists = true; });
   return exists;
}

template<typename... Ts>
void launchCreateRequest(Connector* conn, request_storage::CreateRequestToken token, Ts... args)
{
   launchNoResultRequest(conn, request_storage::createReqList.at(token), args...);
}

template<typename... Ts>
void launchInsertRequest(Connector* conn, request_storage::InsertRequestToken token, Ts... args)
{
   launchNoResultRequest(conn, request_storage::insReqList.at(token), args...);
}

template<typename... Ts>
bool launchSelectRequest(Connector* conn, request_storage::SelectRequestToken token, Ts... args)
{
   return launchResultExistenceRequest(conn, request_storage::selectReqList.at(token), args...);
}

SQLiteController::SQLiteController(central_pack_ptr pack)
   : Central(pack)
   , m_connector(std::make_unique<Connector>(pack))
{
   VALID_CHECK_DLL_LOAD("DataShareProvider", "CreateDataShareProvider", m_shareProvider, pack);
   VALID_CHECK_DLL_LOAD("SettingsHandler", "CreateUnitDataSerializer", m_unitDataSerializer);
   VALID_CHECK_DLL_LOAD("SettingsHandler", "CreateJsonSettingsSerializer", m_settingsSerializer);

   //m_connector->Connect(SVGUtils::wstringToString(GetPathStorage()->database_path).c_str());
   //baseCheckCreate();
}

void SQLiteController::Connect(const char* path)
{
   m_connector->Connect(path);
   baseCheckCreate();
}

void SQLiteController::SaveScenarioData(const settings::unit_source_data& unitData, const std::vector<std::vector<double>>& coordGrid)
{
   //m_shareProvider->Share(GetPack()->paths->coordinate_map_path.c_str(), coordGrid);
   SaveAppSettings();
   saveUnitData(unitData);
}

void SQLiteController::SaveAppSettings()
{
   savePathfindingSettings();
   saveResearchSettings();
   saveEnvironmentSettings();
   saveSimulationSettings();
   saveMapSettings();
}

void SQLiteController::LoadScenarioData(settings::unit_source_data& unitData, std::vector<std::vector<double>>& coordGrid)
{
   LoadAppSettings();
   loadUnitData(unitData);
   //m_shareProvider->GetShared(GetPack()->paths->coordinate_map_path.c_str(), coordGrid);
}

void SQLiteController::LoadAppSettings()
{
   loadMapSettings();
   loadSimulationSettings();
   loadEnvironmentSettings();
   loadResearchSettings();
   loadPathfindingSettings();
}

void SQLiteController::SaveDataStandartHashJunction(data_hash::hash_junction& junc)
{
   auto src = std::to_string(junc.source);
   auto dst = std::to_string(junc.destination);
   if (!CheckDataStandartHashJunction(junc))
      launchInsertRequest(m_connector.get(), request_storage::InsertRequestToken::RT_SETDSHJ, src.c_str(), dst.c_str());
   else
      GetCommunicator()->Message(ICommunicator::MessageType::MT_ERROR, "hash junction conflict on %i -> %i", junc.source, junc.destination);
}

bool SQLiteController::CheckDataStandartHashJunction(data_hash::hash_junction& junc)
{
   auto src = std::to_string(junc.source);
   auto dst = std::to_string(junc.destination);
   return launchSelectRequest(m_connector.get(), request_storage::SelectRequestToken::RT_SELDSHJ, src.c_str(), dst.c_str());
}

void SQLiteController::baseCheckCreate()
{
   for (auto& req : request_storage::createReqList)
      m_connector->SQLNoResRequest(req.second.c_str());
}

void SQLiteController::savePathfindingSettings()
{
   NO_RES_PREP;
   //std::string jsonSettings = m_settingsSerializer->ToString(GetSettings()->pth_stt);

   //STT_INS_NORES_RL(request_storage::SettingsType::ST_PATHFINDING, jsonSettings.c_str());
}

void SQLiteController::saveResearchSettings()
{
   NO_RES_PREP;
   //std::string jsonSettings = m_settingsSerializer->ToString(GetSettings()->res_stt);

   //STT_INS_NORES_RL(request_storage::SettingsType::ST_RESEARCH, jsonSettings.c_str());
}

void SQLiteController::saveEnvironmentSettings()
{
   NO_RES_PREP;
   //std::string jsonSettings = m_settingsSerializer->ToString(GetSettings()->env_stt);

   //STT_INS_NORES_RL(request_storage::SettingsType::ST_ENVIRONMENT, jsonSettings.c_str());
}

void SQLiteController::saveSimulationSettings()
{
   NO_RES_PREP;
   //std::string jsonSettings = m_settingsSerializer->ToString(GetSettings()->sim_stt);

   //STT_INS_NORES_RL(request_storage::SettingsType::ST_SIMULATION, jsonSettings.c_str());
}

void SQLiteController::saveMapSettings()
{
   NO_RES_PREP;
   //std::string jsonSettings = m_settingsSerializer->ToString(GetSettings()->map_stt);

   //STT_INS_NORES_RL(request_storage::SettingsType::ST_MAP, jsonSettings.c_str());
}

void SQLiteController::saveUnitData(const settings::unit_source_data& settings)
{
   NO_RES_PREP;
   //std::string jsonSettings = m_unitDataSerializer->ToString(settings);

   //UNT_INSUPD_NORES_RL(jsonSettings.c_str());
}

void SQLiteController::loadPathfindingSettings()
{
   RES_PREP_STR;
   //std::string dat;

   //STT_SEL_DAT_RES_STR_RL(request_storage::SettingsType::ST_PATHFINDING, dat);

   //m_settingsSerializer->FromString(dat.c_str(), GetSettingsModify()->pth_stt);
}

void SQLiteController::loadResearchSettings()
{
   RES_PREP_STR;
   //std::string dat;

   //STT_SEL_DAT_RES_STR_RL(request_storage::SettingsType::ST_RESEARCH, dat);

   //m_settingsSerializer->FromString(dat.c_str(), GetSettingsModify()->res_stt);
}

void SQLiteController::loadEnvironmentSettings()
{
   RES_PREP_STR;
   //std::string dat;

   //STT_SEL_DAT_RES_STR_RL(request_storage::SettingsType::ST_ENVIRONMENT, dat);

   //m_settingsSerializer->FromString(dat.c_str(), GetSettingsModify()->env_stt);
}

void SQLiteController::loadSimulationSettings()
{
   RES_PREP_STR;
   //std::string dat;

   //STT_SEL_DAT_RES_STR_RL(request_storage::SettingsType::ST_SIMULATION, dat);

   //m_settingsSerializer->FromString(dat.c_str(), GetSettingsModify()->sim_stt);
}

void SQLiteController::loadMapSettings()
{
   RES_PREP_STR;
   //std::string dat;

   //STT_SEL_DAT_RES_STR_RL(request_storage::SettingsType::ST_MAP, dat);

   //m_settingsSerializer->FromString(dat.c_str(), GetSettingsModify()->map_stt);
}

void SQLiteController::loadUnitData(settings::unit_source_data& settings)
{
   RES_PREP_STR;
   //std::string dat;

   //UNT_SEL_DAT_RES_STR_RL(dat);

   //m_unitDataSerializer->FromString(dat.c_str(), settings);
}

database::iSVGMDatabaseController* CreateSQLiteDatabaseController(central_pack* pack)
{
   return new database::SQLiteController(pack);
}