#pragma once

#include "colreg/ChartSafetyStructs.h"
#include "common/chart_object.h"
#include "TagList.h"

#include "json/json_wrapper.h"

namespace geojson_save_helper
{
   Json::Value prop_to_json(const colreg::simple_prop& prop)
   {
      Json::Value jprop;
      jprop[tag::key] = prop.key.c_str();
      jprop[tag::val] = prop.val.c_str();
      return jprop;
   }

   Json::Value geopoint_to_json(const colreg::geo_point& point)
   {
      Json::Value jpoint;
      jpoint[tag::lon] = point.lon;
      jpoint[tag::lat] = point.lat;
      return jpoint;
   }

   Json::Value geopointvct_to_json(const std::vector<colreg::geo_point>& points)
   {
      Json::Value jcontour;
      jcontour.resize(static_cast<Json::ArrayIndex>(points.size()));
      for (size_t idx = 0; idx < points.size(); idx++)
         jcontour[static_cast<Json::ArrayIndex>(idx)] = geopoint_to_json(points.at(idx));
      return jcontour;
   }

   Json::Value cou_to_json(const chart_object::chart_object_unit& obj)
   {
      Json::Value jobj;
      jobj[tag::type] = static_cast<unsigned short>(obj.type);
      Json::Value jprops;
      jprops.resize(obj.prop_vct.size());
      for (size_t idx = 0; idx < obj.prop_vct.size(); idx++)
         jprops[idx] = prop_to_json(obj.prop_vct.at(idx));
      jobj[tag::prop_list] = jprops;

      Json::Value jcontours;
      jcontours.resize(obj.geom_contour_vct.size());
      for (size_t idx = 0; idx < obj.geom_contour_vct.size(); idx++)
         jcontours[idx] = geopointvct_to_json(obj.geom_contour_vct.at(idx));
      jobj[tag::contour_list] = jcontours;
      return jobj;
   }

   Json::Value couvct_to_json(const chart_object::chart_object_unit_vct_ref objList)
   {
      Json::Value objs;
      objs.resize(static_cast<Json::ArrayIndex>(objList.size()));
      for (size_t idx = 0; idx < objList.size(); idx++)
         objs[idx] = cou_to_json(objList.at(idx));
      return objs;
   }
}