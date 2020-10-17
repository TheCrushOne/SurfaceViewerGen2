#pragma once

#include "colreg/ChartSafetyStructs.h"
#include "common/chart_object_unit.h"
#include "common/properties.h"
#include "TagList.h"

#include "json/json_wrapper.h"

namespace geojson_save_helper
{
   inline Json::Value prop_to_json(const properties::simple_prop& prop)
   {
      Json::Value jprop;
      jprop[tag::key] = prop.key.c_str();
      jprop[tag::val] = prop.val.c_str();
      return jprop;
   }

   inline Json::Value geopoint_to_json(const CG::geo_point& point)
   {
      Json::Value jpoint;
      jpoint[tag::lon] = point.lon;
      jpoint[tag::lat] = point.lat;
      return jpoint;
   }

   inline Json::Value geopointvct_to_json(const CG::geo_contour& points)
   {
      Json::Value jcontour(Json::arrayValue);
      for (size_t idx = 0; idx < points.size(); idx++)
         jcontour.append(geopoint_to_json(points.at(idx)));
      return jcontour;
   }

   inline Json::Value cou_to_json(const chart_object::chart_object_unit& obj)
   {
      Json::Value jobj;
      jobj[tag::type] = static_cast<unsigned short>(obj.type);
      Json::Value jprops(Json::arrayValue);
      for (const auto& elem : obj.prop_vct)
         jprops.append(prop_to_json(elem));
      jobj[tag::prop_list] = jprops;

      Json::Value jcontours(Json::arrayValue);
      for (const auto& elem : obj.geom_contour_vct)
         jcontours.append(geopointvct_to_json(elem));
      jobj[tag::contour_list] = jcontours;
      return jobj;
   }

   inline Json::Value couvct_to_json(const chart_object::chart_object_unit_vct& objList)
   {
      Json::Value objs(Json::arrayValue);
      for (const auto& elem : objList)
         objs.append(cou_to_json(elem));
      return objs;
   }
}