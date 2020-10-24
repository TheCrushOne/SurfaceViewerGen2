#pragma once

#include "SurfaceInterfaces.h"

namespace SV::serializer
{
   struct iUnitDataSerializer : iReleasable
   {
      virtual bool Serialize(const char*, const settings::unit_source_data&) const = 0;
      virtual bool Deserialize(const char*, settings::unit_source_data&) const = 0;

      virtual const char* ToString(const settings::unit_source_data&) const = 0;
      virtual bool FromString(const char*, settings::unit_source_data&) const = 0;
   };
}

#ifdef UNITDATASERIALIZER_DLL
#define UDATASEREXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define UDATASEREXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" UDATASEREXPRTIMPRT SV::serializer::iUnitDataSerializer* CreateUnitDataSerializer();