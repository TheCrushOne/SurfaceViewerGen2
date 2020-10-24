#pragma once

#include <vector>
#include "common/chart_object_unit.h"

namespace SV::layer_provider
{
   struct layer_chart_object : SV::chart_object::chart_object_unit
   {
      layer_chart_object()
         : SV::chart_object::chart_object_unit()
         , id(INVALID_ID)
      {}
      layer_chart_object(const SV::chart_object::chart_object_unit& obj, chart_object_id id = INVALID_ID)
         : SV::chart_object::chart_object_unit(obj)
         , id(id)
      {}

      chart_object_id id;
   };

   typedef std::vector<layer_chart_object> layer_chart_object_vct;
}