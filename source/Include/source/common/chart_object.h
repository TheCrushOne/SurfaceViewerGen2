#pragma once

#include <vector>
#include "colreg/CommonStructs.h"
#include "colreg/PropertyInterface.h"
#include "colreg/ChartSafetyStructs.h"

namespace chart_object
{
   struct chart_object_unit
   {
      colreg::geo_contour_vct geom_contour_vct;
      colreg::object_props_vct prop_vct;

      colreg::OBJECT_TYPE type;
      colreg::chart_object_id id;
   };

   typedef std::vector<chart_object_unit> chart_object_unit_vct;
   typedef chart_object_unit_vct& chart_object_unit_vct_ref;
}