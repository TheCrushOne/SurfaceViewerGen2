#pragma once

#include "common/base_types.h"
#include "layer_provider/trajectory_point.h"

namespace SV::layer_provider
{
   struct layer_unit_object
   {
      id_type id;
      CG::layer_provider::trajectory_point_vct trajectory;
      CG::layer_provider::trajectory_point_vct control_points;
   };

   typedef std::vector<layer_unit_object> layer_unit_object_vct;
}