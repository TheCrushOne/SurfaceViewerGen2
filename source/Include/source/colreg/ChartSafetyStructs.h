#pragma once

#include "CommonStructs.h"
#include "BaseRef.h"
#include <string>

namespace colreg
{
#define _BIT_(n) (1i64 << n)

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
#pragma pack (push, 1)

   struct geo_point_ex : geo_point
   {
      geo_point_ex(const geo_point& p, double leftXTE, double rightXTE)
         : geo_point(p)
         , left_XTE{ leftXTE }
         , right_XTE{ rightXTE }
      {}

      double left_XTE;  ///< Максимальное боковое смещение слева, мили
      double right_XTE; ///< Максимальное боковое смещение справа, мили
   };

   using geo_points_ex_ref = base_ref<geo_point_ex>;

   struct simple_prop
   {
      simple_prop() = default;

      simple_prop(const char* k, const char* v)
         : key{ k }
         , val{ v }
      {}

      const char* key;
      const char* val;
   };

   struct simple_prop_holder
   {
      simple_prop_holder() = default;

      simple_prop_holder(std::string k, std::string v)
         : key{ k }
         , val{ v }
      {}

      std::string key;
      std::string val;
   };

   using object_props_ref = base_ref<simple_prop>;

   using chart_object_id = unsigned long long;
   //struct chart_object_id
   //{
   //   id_type static_id = INVALID_ID; ///< ID для статических объектов карты
   //   id_type dynamic_id = INVALID_ID; ///< ID динамических объектов карты

   //   bool operator==(const chart_object_id& id) const
   //   {
   //      return static_id == id.static_id && dynamic_id == id.dynamic_id;
   //   }

   //   inline bool is_valid() const
   //   {
   //      if (static_id == INVALID_ID && dynamic_id == INVALID_ID)
   //         return false;
   //      return static_id == INVALID_ID || dynamic_id == INVALID_ID; // объект либо статический, либо динамический
   //   }
   //};

   //! Геометрия объекта карты
   using object_geometry_ref = base_ref<geo_points_ref>;

   struct chart_object
   {
      chart_object() = default;

      chart_object(OBJECT_TYPE t, const object_geometry_ref& g, const object_props_ref& p)
         : type{ t }, geom{ g }, props{ p }
      {}

      chart_object(chart_object_id i, OBJECT_TYPE t, const object_geometry_ref& g, const object_props_ref& p)
         : id{ i }, type{ t }, geom{ g }, props{ p }
      {}

      chart_object_id id;
      OBJECT_TYPE     type;

      object_geometry_ref geom;
      object_props_ref    props; ///< Свойства объекта
   };

   using chart_objects_ref = base_ref<chart_object>;
#pragma pack (pop)

   inline bool check_chart_obj_type(colreg::mask_type mask, OBJECT_TYPE type)
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
} //namespace colreg