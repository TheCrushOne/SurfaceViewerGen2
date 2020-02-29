#pragma once

#include "SurfaceInterfaces.h"

namespace colreg
{
   struct iSettingsSerializer : iReleasable
   {
      virtual bool Serialize(const char* filename, const settings::pathfinding_settings& s) const = 0;
      virtual bool Deserialize(const char* filename, settings::pathfinding_settings& s) const = 0;

      virtual bool Serialize(const char* filename, const settings::research_settings& s) const = 0;
      virtual bool Deserialize(const char* filename, settings::research_settings& s) const = 0;

      virtual const char* ToString(const settings::pathfinding_settings& srcStt) const = 0;
      virtual bool FromString(const char* src, settings::pathfinding_settings& dstStt) const = 0;

      virtual const char* ToString(const settings::research_settings& srcStt) const = 0;
      virtual bool FromString(const char* src, settings::research_settings& dstStt) const = 0;
   };
}

#ifdef SETTINGSSERIALIZER_DLL
#define SETSEREXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SETSEREXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SETSEREXPRTIMPRT colreg::iSettingsSerializer* CreateSettingsSerializer();