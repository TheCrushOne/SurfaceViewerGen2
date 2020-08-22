#pragma once

#include <unordered_map>

namespace request_storage
{
   enum class SettingsType : unsigned short
   {
      ST_PATHFINDING = 0,
      ST_RESEARCH,
      ST_ENVIRONMENT,
      ST_SIMULATION,
      ST_MAP,
   };

   const static std::unordered_map<SettingsType, std::string> settingsTokenList = {
      { SettingsType::ST_PATHFINDING, "PATHINDING" },
      { SettingsType::ST_RESEARCH, "RESEARCH" },
      { SettingsType::ST_ENVIRONMENT, "ENVIRONMENT" },
      { SettingsType::ST_SIMULATION, "SIMULATION" },
      { SettingsType::ST_MAP, "MAP" },
   };

   enum class CreateRequestToken : unsigned short
   {
      // Create
      RT_CRSCNSTT = 0,
      RT_CRDSHJ,
      //RT_CRRESSTT,
      //RT_CRUNITLIST,
      RT_CRUNITDATA,
      //RT_CRMAPOBJ,
      //RT_CRMAPSTT,
   };

   enum class InsertRequestToken : unsigned short
   {
      // Insert/Update
      RT_SETSCNSTT,
      RT_SETDSHJ,
      //RT_SETPTHSTT,
      //RT_SETRESSTT,
      //RT_SETMAPSTT,
      RT_SETUNTDAT,
   };

   enum class SelectRequestToken : unsigned short
   {
      // Select
      //RT_SELMAPSTT,
      RT_SELSCNSTT,
      RT_SELUNTDAT,
      RT_SELDSHJ,
   };

   const static std::unordered_map<CreateRequestToken, std::string> createReqList = {
      { CreateRequestToken::RT_CRSCNSTT,
         "CREATE TABLE IF NOT EXISTS scenario_settings("
         "setting_id INTEGER PRIMARY KEY, "
         "setting_token TEXT NOT NULL UNIQUE, "
         "setting_json TEXT NOT NULL)" },
      { CreateRequestToken::RT_CRDSHJ,
         "CREATE TABLE IF NOT EXISTS data_standart_hash_junction("
         "rec_id INTEGER PRIMARY KEY, "
         "source_hash TEXT NOT NULL, "
         "target_hash TEXT NOT NULL)" },
      /*{ RequestToken::RT_CRRESSTT,
         "CREATE TABLE IF NOT EXISTS research_settings("
         "setting_id INTEGER PRIMARY KEY, "
         "setting_parameter TEXT NOT NULL UNIQUE, "
         "setting_value TEXT NOT NULL)" },*/
      /*{ RequestToken::RT_CRUNITLIST,
         "CREATE TABLE IF NOT EXISTS unit_list("
         "unit_id INTEGER PRIMARY KEY, "
         "unit_name TEXT NOT NULL)" },
      { RequestToken::RT_CRUNITDATA,
         "CREATE TABLE IF NOT EXISTS unit_data("
         "data_id INTEGER PRIMARY KEY, "
         "data_unitId INTEGER NOT NULL, "
         "data_row INTEGER, "
         "data_col INTEGER, "
         "data_type INTEGER NOT NULL, "
         "FOREIGN KEY(data_unitId) REFERENCES unit_list(unit_id) ON DELETE CASCADE)" }*,*/
      { CreateRequestToken::RT_CRUNITDATA,
         "CREATE TABLE IF NOT EXISTS unit_data("
         "unit_id INTEGER PRIMARY KEY, "
         "unit_uniqId INTEGER UNIQUE, "
         "unit_json TEXT)" },
      /*{ RequestToken::RT_CRMAPOBJ,
         "CREATE TABLE IF NOT EXISTS map_object("
         "object_id INTEGER PRIMARY KEY, "
         "object_type INTEGER NOT NULL, "
         "object_geometry TEXT)" },
      { RequestToken::RT_CRMAPSTT,
         "CREATE TABLE IF NOT EXISTS map_settings("
         "setting_id INTEGER PRIMARY KEY, "
         "setting_parameter TEXT NOT NULL UNIQUE, "
         "setting_value TEXT NOT NULL)" }*/
   };

   const static std::unordered_map<InsertRequestToken, std::string> insReqList = {
      { InsertRequestToken::RT_SETSCNSTT,
         "INSERT INTO scenario_settings(setting_token, setting_json) "
         "VALUES ('%s', '%s') "
         "ON CONFLICT(setting_token) "
         "DO UPDATE SET setting_json = excluded.setting_json" },
      { InsertRequestToken::RT_SETDSHJ,
         "INSERT INTO data_standart_hash_junction(source_hash, target_hash) "
         "VALUES ('%s', '%s') "
         "ON CONFLICT(source_hash, target_hash) "
         "DO NOTHING" },
   };

   const static std::unordered_map<InsertRequestToken, std::string> insupdReqList = {
      /*{ RequestToken::RT_SETPTHSTT,
         "INSERT INTO pathfinding_settings(setting_parameter, setting_value) "
         "VALUES ('%s', '%s') "
         "ON CONFLICT(setting_parameter) "
         "DO UPDATE SET setting_value = excluded.setting_value" },
      { RequestToken::RT_SETRESSTT,
         "INSERT INTO research_settings(setting_parameter, setting_value) "
         "VALUES ('%s', '%s') "
         "ON CONFLICT(setting_parameter) "
         "DO UPDATE SET setting_value = excluded.setting_value" },
      { RequestToken::RT_SETMAPSTT,
         "INSERT INTO map_settings(setting_parameter, setting_value) "
         "VALUES ('%s', '%s') "
         "ON CONFLICT(setting_parameter) "
         "DO UPDATE SET setting_value = excluded.setting_value" },*/
      { InsertRequestToken::RT_SETUNTDAT,
         "INSERT INTO unit_data(unit_uniqId, unit_json) "
         "VALUES ('1', '%s') "
         "ON CONFLICT(unit_uniqId) "
         "DO UPDATE SET unit_json = excluded.unit_json" },
   };

   const static std::unordered_map<SelectRequestToken, std::string> selectReqList = {
      { SelectRequestToken::RT_SELSCNSTT,
         "SELECT setting_token, setting_json "
         "FROM scenario_settings "
         "WHERE setting_token='%s'" },
      { SelectRequestToken::RT_SELUNTDAT,
         "SELECT unit_uniqId, unit_json "
         "FROM unit_data "
         "WHERE unit_uniqId='1'" },
      { SelectRequestToken::RT_SELDSHJ,
         "SELECT source_hash, target_hash "
         "FROM data_standart_hash_junction "
         "WHERE source_hash='%s', target_hash='%s'" },
   };
}