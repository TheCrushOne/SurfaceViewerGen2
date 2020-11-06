#pragma once

#include "colreg/ChartSafetyStructs.h"
#include "common/chart_object_unit.h"
#include "common/object_type.h"
#include "common/properties.h"
#include "TagList.h"

#include "ThirdParty/json/json.h"

namespace SV::geojson_load_helper
{
   properties::simple_prop json_to_prop(const Json::Value& jprop)
   {
      properties::simple_prop prop;
      prop.key = jprop[tag::key].asString().c_str();
      prop.val = jprop[tag::val].asString().c_str();
      return prop;
   }

   CG::geo_point json_to_geopoint(const Json::Value& jpt)
   {
      CG::geo_point point;
      point.lat = jpt[tag::lat].asDouble();
      point.lon = jpt[tag::lon].asDouble();
      return point;
   }

   CG::geo_contour json_to_geopointvct(const Json::Value& jobj)
   {
      CG::geo_contour contour;
      for (auto& jpt : jobj)
         contour.emplace_back(json_to_geopoint(jpt));
      return contour;
   }

   chart_object::chart_object_unit json_to_cou(const Json::Value& jobj)
   {
      chart_object::chart_object_unit obj;
      obj.type = static_cast<chart_object::OBJECT_TYPE>(jobj[tag::type].asUInt());
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

   Json::Value from_file(LPCSTR filename)
   {
      Json::Value root;
      std::ifstream fStr(filename);

      Json::CharReaderBuilder builder;
      builder["collectComments"] = false;
      JSONCPP_STRING errs;
      bool ok = parseFromStream(builder, fStr, &root, &errs);

      return root;
   }
}