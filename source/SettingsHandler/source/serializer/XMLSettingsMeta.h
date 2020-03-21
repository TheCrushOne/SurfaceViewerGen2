#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

namespace colreg
{
   enum class PathfindingSettingsFieldIndex : unsigned short
   {
      PSFI_LEVEL_ST = 0,
      PSFI_MAXAIRHEIGHT,
      PSFI_MAXLANDHEIGHT,
      PSFI_MINLANDHEIGHT,
      PSFI_MAXLANDANGLE,
      PSFI_DANGEROUSLANDANGLE,
   };

   enum class ResearchSettingsFieldIndex : unsigned short
   {
      RSFI_COUNTRANGE_ST = 0,
      RSFI_LENGTHRANGE_ST,
      RSFI_THREADRANGE_ST,
      RSFI_TASKRANGE_ST,
      RSFI_MIN,
      RSFI_MAX,
      RSFI_STEP,

      RSFI_ITERCOUNT,
      RSFI_MAPSIZE,
      RSFI_MULTITHREADTEST,
      RSFI_SINGLETHREADTEST,
      RSFI_DEBUGLEVEL,
   };

   enum class EnvironmentSettingsFieldIndex : unsigned short
   {
      ESFI_GSC = 0,
      ESFI_MTX,
      ESFI_ANGLE,
      ESFI_SCALE,
      ESFI_ORDBIAS,
      ESFI_ABSBIAS,
   };

   const static std::unordered_map<PathfindingSettingsFieldIndex, std::pair<std::string, std::string>> pathfindingMeta = {
      { PathfindingSettingsFieldIndex::PSFI_LEVEL_ST, { "level_settings", "" } },
      { PathfindingSettingsFieldIndex::PSFI_MAXAIRHEIGHT, { "max_air_height", "height limit(air)" } },
      { PathfindingSettingsFieldIndex::PSFI_MAXLANDHEIGHT, { "max_land_height", "height limit(land)" } },
      { PathfindingSettingsFieldIndex::PSFI_MINLANDHEIGHT, { "min_land_height", "height limit(land)/water level" } },
      { PathfindingSettingsFieldIndex::PSFI_MAXLANDANGLE, { "max_land_angle", "critical overturn angle" } },
      { PathfindingSettingsFieldIndex::PSFI_DANGEROUSLANDANGLE, { "dangerous_land_angle", "angle of overturn" } },
   };

   const static std::unordered_map<ResearchSettingsFieldIndex, std::pair<std::string, std::string>> researchMeta = {
      { ResearchSettingsFieldIndex::RSFI_COUNTRANGE_ST, { "count_range_settings", "" } },
      { ResearchSettingsFieldIndex::RSFI_LENGTHRANGE_ST, { "length_range_settings", "" } },
      { ResearchSettingsFieldIndex::RSFI_MIN, { "min", "minimal value" } },
      { ResearchSettingsFieldIndex::RSFI_MAX, { "max", "maximal value" } },
      { ResearchSettingsFieldIndex::RSFI_STEP, { "step", "step value" } },

      { ResearchSettingsFieldIndex::RSFI_ITERCOUNT, { "iter_count", "experimental iteration count" } },
      { ResearchSettingsFieldIndex::RSFI_MAPSIZE, { "map_size", "size of autogenerated map" } },
      { ResearchSettingsFieldIndex::RSFI_MULTITHREADTEST, { "multi_thread_test", "flag of multi thread test" } },
      { ResearchSettingsFieldIndex::RSFI_SINGLETHREADTEST, { "single_thread_test", "flag of single thread test" } },
      { ResearchSettingsFieldIndex::RSFI_DEBUGLEVEL, { "debug_level", "level of debug trace(gui + file)" } },
   };

   const static std::unordered_map<EnvironmentSettingsFieldIndex, std::pair<std::string, std::string>> environmentMeta = {
      { EnvironmentSettingsFieldIndex::ESFI_GSC, { "geographical_coordinate_system", "" } },
      { EnvironmentSettingsFieldIndex::ESFI_MTX, { "matrix_coordinate_system", "" } },
      { EnvironmentSettingsFieldIndex::ESFI_ANGLE, { "angle", "angle" } },
      { EnvironmentSettingsFieldIndex::ESFI_SCALE, { "scale", "scale" } },
      { EnvironmentSettingsFieldIndex::ESFI_ORDBIAS, { "ordbias", "ordinate bias" } },
      { EnvironmentSettingsFieldIndex::ESFI_ABSBIAS, { "absbias", "abscissa bias" } },
   };
}