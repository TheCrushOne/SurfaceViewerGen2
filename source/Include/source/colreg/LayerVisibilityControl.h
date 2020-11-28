#pragma once

#include <string>
#include <vector>

namespace SV::surface_simulation
{
   struct LayerVisibilityControl
   {
      std::string name;
      bool defvalue;
      std::unordered_map<std::string, LayerVisibilityControl> children;
   };
}