#include "stdafx.h"
#include "SVGenMapDS.h"
#include <fstream>

#include "json/json_wrapper.h"

#include <filesystem>

using namespace data_standart;

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
      return; \
   }

SurfaceViewerGenMapDataStandart::SurfaceViewerGenMapDataStandart(central_pack * pack, LPCWSTR base_folder, navigation_dispatcher::iComService * pService)
   : DataStandart(pack, base_folder, pService)
{}

void SurfaceViewerGenMapDataStandart::resolvePathDee()
{
   std::filesystem::path filePath(m_dataStandartData.folder);
   // TODO: check!!!
   if (filePath.is_relative())
      m_dataStandartData.folder = (std::filesystem::path(m_baseFolder) / filePath).generic_string().c_str();
}

void SurfaceViewerGenMapDataStandart::SetData(const pathfinder::GeoMatrix& rawData)
{
   m_rawdata = pathfinder::GeoMatrix(rawData);
   m_rowCount = m_rawdata.GetRowCount();
   m_colCount = m_rawdata.GetColCount();
   std::filesystem::path path(getPath());
   std::filesystem::create_directories(path);
   saveMetaData();
   saveHeightData();
}

pathfinder::GeoMatrix& SurfaceViewerGenMapDataStandart::GetData()
{
   readMetaData();
   readHeightData();
   mountPathFileStorage();
   m_service->GetSettingsSerializerHolder()->GetSettings(m_storage);
   return m_rawdata;
}

settings::unit_source_data& SurfaceViewerGenMapDataStandart::GetUnitData()
{
   readUnitData();
   return m_unitData;
}

void SurfaceViewerGenMapDataStandart::readMetaData()
{
   std::string metaDataPath = getMetaFilePath();
   std::ifstream i(metaDataPath, std::ios_base::in | std::ios::binary);
   Json::Value j;
   i >> j;
   m_rowCount = j[tag::row_count].asUInt();
   m_colCount = j[tag::col_count].asUInt();
}

void SurfaceViewerGenMapDataStandart::readHeightData()
{
   std::string dataFilePath = getDataFilePath();
   std::ifstream file(dataFilePath, std::ios::in | std::ios::binary);
   m_rawdata.SetRowCount(m_rowCount);
   m_rawdata.SetColCount(m_colCount);
   char* buffer = new char[sizeof(double)];
   for (size_t ridx = 0; ridx < m_rowCount; ridx++)
   {
      for (size_t cidx = 0; cidx < m_colCount; cidx++)
      {
         file.read(buffer, sizeof(double));
         m_rawdata.Set(ridx, cidx, *reinterpret_cast<double*>(buffer));
      }
   }
   delete [] buffer;
   file.close();
}

void SurfaceViewerGenMapDataStandart::readUnitData()
{
   m_service->GetUnitDataSerializer()->Deserialize(m_dataStandartData.unit_data.c_str(), m_unitData);
}

void SurfaceViewerGenMapDataStandart::saveMetaData()
{
   std::string metaDataPath = getMetaFilePath();
   std::ofstream o(metaDataPath);
   Json::Value j;
   j[tag::row_count] = m_rowCount;
   j[tag::col_count] = m_colCount;
   o << j;
}

void SurfaceViewerGenMapDataStandart::saveHeightData()
{
   std::string dataFilePath = getDataFilePath();
   std::ofstream file(dataFilePath, std::ios::out | std::ios::binary);
   for (size_t ridx = 0; ridx < m_rowCount; ridx++)
   {
      for (size_t cidx = 0; cidx < m_colCount; cidx++)
      {
         double buffer = m_rawdata.Get(ridx, cidx);
         file.write(reinterpret_cast<const char*>(&buffer), sizeof(double));
      }
   }
   file.close();
}

void SurfaceViewerGenMapDataStandart::mountPathFileStorage()
{
   m_storage.environment_settings_path.condition_mount(SVGUtils::stringToWstring(m_dataStandartData.env_stt), !m_dataStandartData.env_stt.empty());
   m_storage.map_settings_path.condition_mount(SVGUtils::stringToWstring(m_dataStandartData.map_stt), !m_dataStandartData.map_stt.empty());
   m_storage.pathfinder_settings_path.condition_mount(SVGUtils::stringToWstring(m_dataStandartData.pth_stt), !m_dataStandartData.pth_stt.empty());
   m_storage.research_settings_path.condition_mount(SVGUtils::stringToWstring(m_dataStandartData.res_stt), !m_dataStandartData.res_stt.empty());
   m_storage.simulation_settings_path.condition_mount(SVGUtils::stringToWstring(m_dataStandartData.sim_stt), !m_dataStandartData.sim_stt.empty());
}

iDataStandart* CreateSurfaceViewerGenMapDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new SurfaceViewerGenMapDataStandart(pack, base_folder, pService);
}