#pragma once

#include "colreg/ChartSafetyStructs.h"
#include "common/chart_object.h"
#include "TagList.h"

#include "ThirdParty/json/json.h"

namespace geojson_load_helper
{
   colreg::simple_prop json_to_prop(const Json::Value& jprop)
   {
      colreg::simple_prop prop;
      prop.key = jprop[tag::key].asString().c_str();
      prop.val = jprop[tag::val].asString().c_str();
      return prop;
   }

   SVCG::geo_point json_to_geopoint(const Json::Value& jpt)
   {
      SVCG::geo_point point;
      point.lat = jpt[tag::lat].asDouble();
      point.lon = jpt[tag::lon].asDouble();
      return point;
   }

   std::vector<SVCG::geo_point> json_to_geopointvct(const Json::Value& jobj)
   {
      std::vector<SVCG::geo_point> contour;
      for (auto& jpt : jobj)
         contour.emplace_back(json_to_geopoint(jpt));
      return contour;
   }

   chart_object::chart_object_unit json_to_cou(const Json::Value& jobj)
   {
      chart_object::chart_object_unit obj;
      obj.type = static_cast<colreg::OBJECT_TYPE>(jobj[tag::type].asUInt());
      for (const auto& jrp : jobj[tag::contour_list])
         obj.geom_contour_vct.emplace_back(json_to_geopointvct(jrp));
      for (const auto& jrp : jobj[tag::prop_list])
         obj.prop_vct.emplace_back(json_to_prop(jrp));
      return obj;
   }

   chart_object::chart_object_unit_vct json_to_couvct(const Json::Value& jobj_list)
   {
      chart_object::chart_object_unit_vct rest;
      for (const auto& jobj : jobj_list)
         rest.emplace_back(json_to_cou(jobj));
      return rest;
   }

   Json::Value from_file(LPCWSTR filename)
   {
      Json::Value root;
      std::ifstream fStr(filename);

      fStr >> root;
      return root;
   }
}