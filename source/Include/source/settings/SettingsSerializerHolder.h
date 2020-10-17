#pragma once

#include "settings.h"
#include "common\file_storage.h"

namespace SV::settings
{
   struct iSettingsSerializerHolder
   {
      virtual application_settings& GetSettings() = 0;
      virtual application_settings& GetSettings(const file_utils::global_path_storage&) = 0;
   };
}