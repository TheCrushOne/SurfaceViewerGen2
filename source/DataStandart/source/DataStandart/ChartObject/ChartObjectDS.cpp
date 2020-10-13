#include "stdafx.h"
#include "ChartObjectDS.h"

#include <fstream>
#include <filesystem>

#include "Common\GeoJsonLoadHelper.h"
#include "Common\GeoJsonSaveHelper.h"

using namespace data_standart;

void ChartObjectDataStandart::resolvePathDee()
{
   std::filesystem::path filePath(m_dataStandartData.folder);
   if (filePath.is_relative())
      m_dataStandartData.folder = (std::filesystem::path(m_baseFolder) / filePath).generic_string().c_str();
}

void ChartObjectDataStandart::SetData(const chart_object::chart_object_unit_vct_ref staticObjVec, const chart_object::chart_object_unit_vct_ref dynamicObjVec)
{
   m_objectVecJunc.static_objects = staticObjVec;
   m_objectVecJunc.dynamic_objects = dynamicObjVec;
   std::filesystem::path path(getPath());
   std::filesystem::create_directories(path);
   saveObjectData();
}

void ChartObjectDataStandart::saveObjectData()
{
   std::string dataFilePath = getDataFilePath();
   std::wstring wDat = SVGUtils::stringToWstring(dataFilePath);

   //m_service->GetPythonWrapper()->RunScript();

   Json::Value root;
   root[tag::static_obj] = geojson_save_helper::couvct_to_json(m_objectVecJunc.static_objects);
   root[tag::dynamic_obj] = geojson_save_helper::couvct_to_json(m_objectVecJunc.dynamic_objects);

   json_wrapper::to_file(root, wDat.c_str());
   root.clear();
}

void ChartObjectDataStandart::readObjectData()
{
   m_objectVecJunc.static_objects.clear();
   m_objectVecJunc.dynamic_objects.clear();
   std::string dataFilePath = getDataFilePath();
   std::wstring wDat = SVGUtils::stringToWstring(dataFilePath);

   Json::Value root = geojson_load_helper::from_file(wDat.c_str());

   m_objectVecJunc.static_objects = geojson_load_helper::json_to_couvct(root[tag::static_obj]);
   m_objectVecJunc.dynamic_objects = geojson_load_helper::json_to_couvct(root[tag::dynamic_obj]);
   root.clear();
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