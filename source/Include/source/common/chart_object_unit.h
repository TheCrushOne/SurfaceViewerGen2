#pragma once

#include <vector>
#include "common/properties.h"
#include "common/object_type.h"
#include "SVCG/geo_point.h"

#define _BIT_(n) (1i64 << n)

namespace SV::chart_object
{
   struct chart_object_unit
   {
      CG::geo_contour_vct geom_contour_vct;
      properties::simple_prop_vct prop_vct;

      OBJECT_TYPE type;
   };

   typedef std::vector<chart_object_unit> chart_object_unit_vct;
}