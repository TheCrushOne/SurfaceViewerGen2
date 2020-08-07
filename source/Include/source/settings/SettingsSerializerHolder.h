#pragma once

#include "settings.h"

namespace settings
{
   struct iSettingsSerializerHolder
   {
      virtual application_settings& GetSettings() = 0;
   };
}