#pragma once

#include "common/properties.h"

namespace SV::surface_simulation
{
   struct iLayerEntity
   {
      virtual properties::simple_prop_vct* GetProps() const = 0;
   };
}