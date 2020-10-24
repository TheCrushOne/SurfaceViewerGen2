#pragma once

#include "CommonStructs.h"
#include "BaseRef.h"
#include <string>
#include <vector>

namespace SV::chart_safety
{
#pragma pack (push, 1)

   //struct geo_point_ex : geo_point
   //{
   //   geo_point_ex(const geo_point& p, double leftXTE, double rightXTE)
   //      : geo_point(p)
   //      , left_XTE{ leftXTE }
   //      , right_XTE{ rightXTE }
   //   {}

   //   double left_XTE;  ///< Максимальное боковое смещение слева, мили
   //   double right_XTE; ///< Максимальное боковое смещение справа, мили
   //};

   //using geo_points_ex_ref = base_ref<geo_point_ex>;

   /*struct simple_prop
   {
      simple_prop() = default;

      simple_prop(const char* k, const char* v)
         : key{ k }
         , val{ v }
      {}

      const char* key;
      const char* val;
   };*/

   

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
   //using object_geometry_ref = base_ref<geo_points_ref>;
   

   /*struct chart_object
   {
      chart_object() = default;

      chart_object(OBJECT_TYPE t, const object_geometry_vct_ref g, const object_props_vct_ref p)
         : type{ t }, geom{ g }, props{ p }
      {}

      chart_object(chart_object_id i, OBJECT_TYPE t, const object_geometry_vct_ref g, const object_props_vct_ref p)
         : id{ i }, type{ t }, geom{ g }, props{ p }
      {}

      chart_object_id id;
      OBJECT_TYPE     type;

      object_geometry_vct geom;
      object_props_vct    props; ///< Свойства объекта
   };*/

   /*typedef std::vector<chart_object> chart_object_vct;
   typedef chart_object_vct& chart_object_vct_ref;*/
   //using chart_objects_ref = base_ref<chart_object>;
#pragma pack (pop)
} //namespace colreg