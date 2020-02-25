#pragma once

#include <unordered_map>

namespace request_storage
{
   enum class RequestToken : unsigned short
   {
      RT_CRPTHSTT= 0,
      RT_CRRESSTT,
      RT_CRUNITLIST,
      RT_CRUNITDATA,
      RT_CRMAPOBJ,
      RT_CRMAPMETA,

      RT_SETPTHSTT,
      RT_SETRESSTT,
   };

   const static std::unordered_map<RequestToken, std::string> createReqList = {
      { RequestToken::RT_CRPTHSTT,
         "CREATE TABLE IF NOT EXISTS pathfinding_settings("
         "setting_id INTEGER PRIMARY KEY,"
         "setting_parameter TEXT NOT NULL,"
         "setting_value TEXT NOT NULL)" },
      { RequestToken::RT_CRRESSTT,
         "CREATE TABLE IF NOT EXISTS research_settings("
         "setting_id INTEGER PRIMARY KEY,"
         "setting_parameter TEXT NOT NULL,"
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
         "object_type INTEGER NOT NULL,"
         "object_geometry TEXT)" },
      { RequestToken::RT_CRMAPMETA,
         "CREATE TABLE IF NOT EXISTS map_meta("
         "meta_id INTEGER PRIMARY KEY, "
         "meta_parameter TEXT NOT NULL)" }
   };

   const static std::unordered_map<RequestToken, std::string> settingsReqList = {
      { RequestToken::RT_SETPTHSTT,
         "INSERT INTO pathfinding_settings(setting_parameter, setting_value) "
         "VALUES (%s, %s)" },
      { RequestToken::RT_SETPTHSTT,
         "INSERT INTO research_settings(setting_parameter, setting_value) "
         "VALUES (%s, %s)" }
   };
}