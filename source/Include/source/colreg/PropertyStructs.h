#pragma once

#include <string>

namespace SV
{
   enum class PROPERTY_TYPE
   {
      PT_VALUE = 0,
      PT_BOOL,
      PT_TEXT,
      PT_TEXT_LIST,
      PT_COLOR,
      PT_FOLRDER
   };

   enum class VALUE_FORMAT_TYPE
   {
      VFT_NONE = 0,
      VFT_SPEED,
      VFT_SPEED_MS,
      VFT_COURSE,
      VFT_DISTANCE,
      VFT_DISTANCE_METERS,
      VFT_TIME_SEC,
      VFT_COORD_LAT,
      VFT_COORD_LON,
      VFT_PERCENT,

      VFT_LOGFILE,
      VFT_FOLDERPATH,
   };

   struct FieldMeta
   {
      std::string name;
      std::string description;
      VALUE_FORMAT_TYPE type;
      bool readOnly;
   };
}