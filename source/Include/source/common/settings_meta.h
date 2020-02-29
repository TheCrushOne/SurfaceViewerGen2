#pragma once

#include <unordered_map>

namespace settings
{
   enum class PathfindingSettingsUID : unsigned short
   {
      PSU_MAXAIRHEIGHT = 0,
      PSU_MAXLANDHEIGHT,
      PSU_MINLANDHEIGHT,
      PSU_MAXLANDANGLE,
      PSU_DANGEROUSLANDANGLE,
   };

   enum class ResearchSettingsUID : unsigned short
   {
      RSU_COUNTRANGEMIN = 0,
      RSU_COUNTRANGESTEP,
      RSU_COUNTRANGEMAX,

      RSU_LENGTHRANGEMIN,
      RSU_LENGTHRANGESTEP,
      RSU_LENGTHRANGEMAX,
   };

   enum class MapSettingsUID : unsigned short
   {
      MSU_ROWCOUNT = 0,
      MSU_COLCOUNT,
   };

   const static std::unordered_map<PathfindingSettingsUID, std::string> pathfindingSettingsMeta = {
      { PathfindingSettingsUID::PSU_MAXAIRHEIGHT, "max_air_height" },
      { PathfindingSettingsUID::PSU_MAXLANDHEIGHT, "max_land_height" },
      { PathfindingSettingsUID::PSU_MINLANDHEIGHT, "min_land_height" },
      { PathfindingSettingsUID::PSU_MAXLANDANGLE, "max_land_angle" },
      { PathfindingSettingsUID::PSU_DANGEROUSLANDANGLE, "dangerous_land_angle" },
   };

   const static std::unordered_map<ResearchSettingsUID, std::string> researchSettingsMeta = {
      { ResearchSettingsUID::RSU_COUNTRANGEMIN, "count_range_min" },
      { ResearchSettingsUID::RSU_COUNTRANGESTEP, "count_range_step" },
      { ResearchSettingsUID::RSU_COUNTRANGEMAX, "count_range_max" },
      { ResearchSettingsUID::RSU_LENGTHRANGEMIN, "length_range_min" },
      { ResearchSettingsUID::RSU_LENGTHRANGESTEP, "length_range_step" },
      { ResearchSettingsUID::RSU_LENGTHRANGEMAX, "length_range_max" },
   };

   const static std::unordered_map<MapSettingsUID, std::string> mapSettingsMeta = {
      { MapSettingsUID::MSU_ROWCOUNT, "row_count" },
      { MapSettingsUID::MSU_COLCOUNT, "col_count" },
   };
}