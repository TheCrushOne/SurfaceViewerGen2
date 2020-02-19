#pragma once

#include "SurfaceInterfaces.h"

namespace colreg
{
   struct iUnitDataSerializer : iReleasable
   {
      virtual bool Serialize(const char* filename, const settings::unit_settings& s) const = 0;
      virtual bool Deserialize(const char* filename, settings::unit_settings& s) const = 0;

      virtual const char* ToString(const settings::unit_settings& srcStt) const = 0;
      virtual bool FromString(const char* src, settings::unit_settings& dstStt) const = 0;
   };
}

extern "C" EXPRTIMPRT colreg::iUnitDataSerializer* CreateUnitDataSerializer();