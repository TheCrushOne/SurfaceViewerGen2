#pragma once

#include "common/base_types.h"

#define _BIT_(n) (1i64 << n)

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

   inline bool check_chart_obj_type(mask_type mask, OBJECT_TYPE type)
   {
      return (mask & type) != 0;
   }

   inline const char* chart_obj_type_to_str(OBJECT_TYPE type)
   {
      switch (type)
      {
         // SurfaceViewer Types
         case OBJECT_TYPE::OT_BORDER_AREA:   return "B_AREA";
         case OBJECT_TYPE::OT_ISOLINE:       return "ISOLINE";
         case OBJECT_TYPE::OT_NO_GO_AREA:    return "NOGO";
         case OBJECT_TYPE::OT_NO_FLY_AREA:   return "NOFLY";
      }
      return "UNKNOWN";
   }
}