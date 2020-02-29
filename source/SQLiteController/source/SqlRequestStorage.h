#pragma once

#include <unordered_map>

namespace request_storage
{
   enum class RequestToken : unsigned short
   {
      // Create
      RT_CRPTHSTT= 0,
      RT_CRRESSTT,
      RT_CRUNITLIST,
      RT_CRUNITDATA,
      RT_CRMAPOBJ,
      RT_CRMAPSTT,

      // Insert/Update
      RT_SETPTHSTT,
      RT_SETRESSTT,
      RT_SETMAPSTT,

      // Select
      RT_SELMAPSTT,
   };

   const static std::unordered_map<RequestToken, std::string> createReqList = {
      { RequestToken::RT_CRPTHSTT,
         "CREATE TABLE IF NOT EXISTS pathfinding_settings("
         "setting_id INTEGER PRIMARY KEY, "
         "setting_parameter TEXT NOT NULL UNIQUE, "
         "setting_value TEXT NOT NULL)" },
      { RequestToken::RT_CRRESSTT,
         "CREATE TABLE IF NOT EXISTS research_settings("
         "setting_id INTEGER PRIMARY KEY, "
         "setting_parameter TEXT NOT NULL UNIQUE, "
         "setting_value TEXT NOT NULL)" },
      { RequestToken::RT_CRUNITLIST,
         "CREATE TABLE IF NOT EXISTS unit_list("
         "unit_id INTEGER PRIMARY KEY, "
         "unit_name TEXT NOT NULL)" },
      { RequestToken::RT_CRUNITDATA,
         "CREATE TABLE IF NOT EXISTS unit_data("
         "data_id INTEGER PRIMARY KEY, "
         "data_unitId INTEGER NOT NULL, "
         "data_x DOUBLE, "
         "data_y DOUBLE, "
         "data_type INTEGER NOT NULL, "
         "FOREIGN KEY(data_unitId) REFERENCES unit_list(unit_id) ON DELETE CASCADE)" },
      { RequestToken::RT_CRMAPOBJ,
         "CREATE TABLE IF NOT EXISTS map_object("
         "object_id INTEGER PRIMARY KEY, "
         "object_type INTEGER NOT NULL, "
         "object_geometry TEXT)" },
      { RequestToken::RT_CRMAPSTT,
         "CREATE TABLE IF NOT EXISTS map_settings("
         "setting_id INTEGER PRIMARY KEY, "
         "setting_parameter TEXT NOT NULL UNIQUE, "
         "setting_value TEXT NOT NULL)" }
   };

   const static std::unordered_map<RequestToken, std::string> insupdReqList = {
      { RequestToken::RT_SETPTHSTT,
         "INSERT INTO pathfinding_settings(setting_parameter, setting_value) "
         "VALUES ('%s', '%s') "
         "ON CONFLICT(setting_parameter) "
         "DO UPDATE SET setting_value = excluded.setting_value" },
      { RequestToken::RT_SETPTHSTT,
         "INSERT INTO research_settings(setting_parameter, setting_value) "
         "VALUES ('%s', '%s')"
         "ON CONFLICT(setting_parameter) "
         "DO UPDATE SET setting_value = excluded.setting_value" },
      { RequestToken::RT_SETMAPSTT,
         "INSERT INTO map_settings(setting_parameter, setting_value) "
         "VALUES ('%s', '%s')"
         "ON CONFLICT(setting_parameter) "
         "DO UPDATE SET setting_value = excluded.setting_value" },
   };

   const static std::unordered_map<RequestToken, std::string> selectReqList = {
      { RequestToken::RT_SELMAPSTT,
         "SELECT 'setting_parameter', 'setting_value' "
         "FROM 'map_settings'"
         "WHERE setting_parameter='%s'" },
   };
}