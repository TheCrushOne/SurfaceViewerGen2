#pragma once

#include <vector>
#include "common/chart_object.h"

namespace SV::layer_provider
{
   struct layer_chart_object : SV::chart_object::chart_object_unit
   {
      CG::chart_object_id id;
   };

   typedef std::vector<layer_chart_object> layer_chart_object_vct;
}