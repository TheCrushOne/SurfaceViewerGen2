#pragma once

#include "ColregInterfaces.h"
#include "WeatherInterface.h"

namespace colreg
{
   struct iSettingsSerializer : iReleasable
   {
      virtual bool Serialize(const char* filename, const settings& s) const = 0;
      virtual bool Deserialize(const char* filename, settings& s) const = 0;

      virtual bool Serialize(const char* filename, const environment& s) const = 0;
      virtual bool Deserialize(const char* filename, environment& s) const = 0;

      virtual const char* ToString(const settings& srcStt) const = 0;
      virtual bool FromString(const char* src, settings& dstStt) const = 0;

      virtual const char* ToString(const environment& srcStt) const = 0;
      virtual bool FromString(const char* src, environment& dstStt) const = 0;
   };
}

extern "C" EXPRTIMPRT colreg::iSettingsSerializer* CreateSettingsSerializer();