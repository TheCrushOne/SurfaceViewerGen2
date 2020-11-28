#pragma once

#include <string>
#include <vector>

namespace SV::surface_simulation
{
   struct LayerVisibilityControl
   {
      std::string name;
      std::vector<LayerVisibilityControl> children;
   };
}