#pragma once

#include <vector>
#include "colreg/CommonStructs.h"
#include "colreg/PropertyInterface.h"
#include "colreg/ChartSafetyStructs.h"
#include "SVCG/base_structs.h"

namespace SV::chart_object
{
   //! Типы опасных объектов карты
   enum OBJECT_TYPE : unsigned long long int
   {
      OT_NONE = 0,

      OT_BORDER_AREA = _BIT_(0),
      OT_ISOLINE = _BIT_(1),
      OT_NO_GO_AREA = _BIT_(2),
      OT_NO_FLY_AREA = _BIT_(3),

      OT_FORBIDDEN_AREAS = OT_NO_GO_AREA | OT_NO_FLY_AREA,
      OT_AREAS = OT_BORDER_AREA | OT_NO_GO_AREA | OT_NO_FLY_AREA,
      OT_LINES = OT_ISOLINE,

      OT_ALL = 0xFFFFFFFFFFFFFFFF
   };

   struct chart_object_unit
   {
      CG::geo_contour_vct geom_contour_vct;
      colreg::object_props_vct prop_vct;

      OBJECT_TYPE type;
   };

   typedef std::vector<chart_object_unit> chart_object_unit_vct;
}