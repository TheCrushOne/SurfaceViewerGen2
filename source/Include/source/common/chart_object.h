#pragma once

#include <vector>
#include "colreg/CommonStructs.h"
#include "colreg/PropertyInterface.h"

namespace chart_object
{
   struct geometry_chart_object
   {
      std::vector<std::vector<colreg::geo_point>> geom_contour_vct;
      std::vector<colreg::simple_prop> prop_vct;

      colreg::OBJECT_TYPE type;
   };
}