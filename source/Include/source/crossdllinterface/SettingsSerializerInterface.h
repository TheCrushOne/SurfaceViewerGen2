#pragma once

#include "colreg/BaseRef.h"

namespace SV::serializer
{
   struct iSettingsSerializer : iReleasable
   {
      virtual bool Serialize(const char*, const settings::pathfinding_settings&) const = 0;
      virtual bool Deserialize(const char*, settings::pathfinding_settings&) const = 0;

      virtual bool Serialize(const char*, const settings::research_settings&) const = 0;
      virtual bool Deserialize(const char*, settings::research_settings&) const = 0;

      virtual bool Serialize(const char*, const settings::environment_settings&) const = 0;
      virtual bool Deserialize(const char*, settings::environment_settings&) const = 0;

      virtual bool Serialize(const char*, const settings::simulation_settings&) const = 0;
      virtual bool Deserialize(const char*, settings::simulation_settings&) const = 0;

      virtual bool Serialize(const char*, const settings::map_settings&) const = 0;
      virtual bool Deserialize(const char*, settings::map_settings&) const = 0;



      virtual const char* ToString(const settings::pathfinding_settings&) const = 0;
      virtual bool FromString(const char*, settings::pathfinding_settings&) const = 0;

      virtual const char* ToString(const settings::research_settings&) const = 0;
      virtual bool FromString(const char*, settings::research_settings&) const = 0;

      virtual const char* ToString(const settings::environment_settings&) const = 0;
      virtual bool FromString(const char*, settings::environment_settings&) const = 0;

      virtual const char* ToString(const settings::simulation_settings&) const = 0;
      virtual bool FromString(const char*, settings::simulation_settings&) const = 0;

      virtual const char* ToString(const settings::map_settings&) const = 0;
      virtual bool FromString(const char*, settings::map_settings&) const = 0;
   };
}

#ifdef SETTINGSSERIALIZER_DLL
#define SETSEREXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SETSEREXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SETSEREXPRTIMPRT SV::serializer::iSettingsSerializer * CreateXMLSettingsSerializer();
extern "C" SETSEREXPRTIMPRT SV::serializer::iSettingsSerializer * CreateJsonSettingsSerializer();