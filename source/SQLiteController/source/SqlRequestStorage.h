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

   enum class RequestToken : unsigned short
   {
      // Create
      RT_CRSCNSTT = 0,
      //RT_CRRESSTT,
      //RT_CRUNITLIST,
      //RT_CRUNITDATA,
      //RT_CRMAPOBJ,
      //RT_CRMAPSTT,

      // Insert/Update
      RT_SETSCNSTT,
      //RT_SETPTHSTT,
      //RT_SETRESSTT,
      //RT_SETMAPSTT,
      RT_SETUNTDAT,

      // Select
      //RT_SELMAPSTT,
      RT_SELSCNSTT,
      RT_SELUNTDAT,
   };

   const static std::unordered_map<RequestToken, std::string> createReqList = {
      { RequestToken::RT_CRSCNSTT,
         "CREATE TABLE IF NOT EXISTS scenario_settings("
         "setting_id INTEGER PRIMARY KEY, "
         "setting_token TEXT NOT NULL UNIQUE, "
         "setting_json TEXT NOT NULL)" },
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
      /*{ RequestToken::RT_CRUNITDATA,
         "CREATE TABLE IF NOT EXISTS unit_data("
         "unit_id INTEGER PRIMARY KEY, "
         "unit_uniqId INTEGER UNIQUE, "
         "unit_json TEXT)" },
      { RequestToken::RT_CRMAPOBJ,
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

   const static std::unordered_map<RequestToken, std::string> insReqList = {
      { RequestToken::RT_SETSCNSTT,
         "INSERT INTO scenario_settings(setting_token, setting_json) "
         "VALUES ('%s', '%s') "
         "ON CONFLICT(setting_token) "
         "DO UPDATE SET setting_json = excluded.setting_json" },
   };

   const static std::unordered_map<RequestToken, std::string> insupdReqList = {
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
      { RequestToken::RT_SETUNTDAT,
         "INSERT INTO unit_data(unit_uniqId, unit_json) "
         "VALUES ('1', '%s') "
         "ON CONFLICT(unit_uniqId) "
         "DO UPDATE SET unit_json = excluded.unit_json" },
   };

   const static std::unordered_map<RequestToken, std::string> selectReqList = {
      { RequestToken::RT_SELSCNSTT,
         "SELECT setting_token, setting_json "
         "FROM scenario_settings "
         "WHERE setting_token='%s'" },
      { RequestToken::RT_SELUNTDAT,
         "SELECT unit_uniqId, unit_json "
         "FROM unit_data "
         "WHERE unit_uniqId='1'" },
   };
}