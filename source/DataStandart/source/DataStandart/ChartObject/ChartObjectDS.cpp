#include "stdafx.h"
#include "ChartObjectDS.h"

#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace data_standart;

void ChartObjectDataStandart::resolvePathDee()
{
   std::filesystem::path filePath(m_dataStandartData.folder);
   if (filePath.is_relative())
      m_dataStandartData.folder = (std::filesystem::path(m_baseFolder) / filePath).generic_string().c_str();
}

void ChartObjectDataStandart::SetData(const geom_object_vec& staticObjVec, const geom_object_vec& dynamicObjVec)
{
   saveObjectData();
}

void ChartObjectDataStandart::saveObjectData()
{
   std::string dataFilePath = getDataFilePath();
   std::ofstream file(dataFilePath);
   json j;
   auto propWriter = [](const colreg::simple_prop_holder& prop)->json
   {
      json jprop;
      jprop[tag::key] = prop.key;
      jprop[tag::val] = prop.val;
      return jprop;
   };
   auto routePointWrite = [](const colreg::geo_point& point)->json
   {
      json pt;
      pt[tag::lon] = point.lon;
      pt[tag::lat] = point.lat;
      return pt;
   };
   auto geomContourWriter = [routePointWrite](const std::vector<colreg::geo_point>& points)->json
   {
      json contour;
      for (auto& point : points)
         contour.emplace_back(routePointWrite(point));
      return contour;
   };
   auto objectWriter = [propWriter, geomContourWriter](const chart_object::geometry_chart_object& obj)->json
   {
      json jobj;
      jobj[tag::type] = static_cast<unsigned short>(obj.type);
      json jprops;
      for (auto& prop : obj.prop_vct)
         jprops.emplace_back(propWriter(prop));
      jobj[tag::prop_list] = jprops;
      json jcontours;
      for (auto& contour : obj.geom_contour_vct)
         jcontours.emplace_back(geomContourWriter(contour));
      jobj[tag::contour_list] = jcontours;
      return jobj;
   };
   auto objectListWriter = [objectWriter](const std::vector<chart_object::geometry_chart_object>& objList)->json
   {
      json objs;
      for (auto& obj : objList)
         objs.emplace_back(objectWriter(obj));
      return objs;
   };
   j[tag::static_obj] = objectListWriter(m_objectVecJunc.static_objects);
   j[tag::dynamic_obj] = objectListWriter(m_objectVecJunc.dynamic_objects);
   file << j;
}

void ChartObjectDataStandart::readObjectData()
{
   m_objectVecJunc.static_objects.clear();
   m_objectVecJunc.dynamic_objects.clear();
   std::string dataFilePath = getDataFilePath();
   std::ifstream file(dataFilePath);
   json j;
   file >> j;
   auto propReader = [](const json& jprop)->colreg::simple_prop_holder
   {
      colreg::simple_prop_holder prop;
      prop.key = jprop[tag::key].get<std::string>();
      prop.val = jprop[tag::val].get<std::string>();
      return prop;
   };
   auto routePointReader = [](const json& jpt)->colreg::geo_point
   {
      colreg::geo_point point;
      point.lat = jpt[tag::lat].get<double>();
      point.lon = jpt[tag::lon].get<double>();
      return point;
   };
   auto geomContourReader = [routePointReader](const json& jobj)->std::vector<colreg::geo_point>
   {
      std::vector<colreg::geo_point> contour;
      for (auto& jpt : jobj)
         contour.emplace_back(routePointReader(jpt));
      return contour;
   };
   auto objectReader = [geomContourReader, propReader](const json& jobj)->chart_object::geometry_chart_object
   {
      chart_object::geometry_chart_object obj;
      obj.type = static_cast<colreg::OBJECT_TYPE>(jobj[tag::type].get<unsigned short>());
      for (auto& jrp : jobj[tag::contour_list])
         obj.geom_contour_vct.emplace_back(geomContourReader(jrp));
      for (auto& jrp : jobj[tag::contour_list])
         obj.prop_vct.emplace_back(propReader(jrp));
      return obj;
   };
   auto objListReader = [objectReader](geom_object_vec& obj_list, const json& jobj_list)
   {
      for (auto& jobj : jobj_list)
         obj_list.emplace_back(objectReader(jobj));
   };
   objListReader(m_objectVecJunc.static_objects, j[tag::static_obj]);
   objListReader(m_objectVecJunc.dynamic_objects, j[tag::dynamic_obj]);
}

const geom_object_vec_junc& ChartObjectDataStandart::GetData()
{
   readObjectData();
   return m_objectVecJunc;
}

iDataStandart* CreateChartObjectDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new ChartObjectDataStandart(pack, base_folder, pService);
}