#include "stdafx.h"
#include "SVGenMapDS.h"
#include <fstream>
#include <nlohmann/json.hpp>

#include <filesystem>

using namespace nlohmann;
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
{
   VALID_CHECK_DLL_LOAD("SettingsHandler", "CreateUnitDataSerializer", m_unitDataSerializer);
   VALID_CHECK_DLL_LOAD("SettingsHandler", "CreateJsonSettingsSerializer", m_settingsSerializer);
}

void SurfaceViewerGenMapDataStandart::resolvePathDee()
{
   std::filesystem::path filePath(m_dataStandartData.folder);
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
   return m_rawdata;
}

settings::unit_source_data& SurfaceViewerGenMapDataStandart::GetUnitData()
{
   readUnitData();
   return m_unitData;
}

settings::application_settings& SurfaceViewerGenMapDataStandart::GetSettings()
{
   readSettings();
   return m_settings;
}

void SurfaceViewerGenMapDataStandart::readMetaData()
{
   std::string metaDataPath = getMetaFilePath();
   std::ifstream i(metaDataPath, std::ios_base::in | std::ios::binary);
   json j;
   i >> j;
   m_rowCount = j["row_count"].get<size_t>();
   m_colCount = j["col_count"].get<size_t>();
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
   m_unitDataSerializer->Deserialize(m_dataStandartData.unit_data.c_str(), m_unitData);
}

#define STT_DESERIALIZE(type, msg_part) \
   if (!m_dataStandartData.type.empty()) \
      m_settingsSerializer->Deserialize(m_dataStandartData.type.c_str(), m_settings.type); \
   else \
      GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "type settings skipped to default");

void SurfaceViewerGenMapDataStandart::readSettings()
{
   STT_DESERIALIZE(env_stt, "Environment");
   STT_DESERIALIZE(map_stt, "Map");
   STT_DESERIALIZE(pth_stt, "Pathfinding");
   STT_DESERIALIZE(res_stt, "Research");
   STT_DESERIALIZE(sim_stt, "Simulation");
}

#undef STT_DESERIALIZE

void SurfaceViewerGenMapDataStandart::saveMetaData()
{
   std::string metaDataPath = getMetaFilePath();
   std::ofstream o(metaDataPath);
   json j;
   j["row_count"] = m_rowCount;
   j["col_count"] = m_colCount;
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

size_t SurfaceViewerGenMapDataStandart::GetDataHash()
{
   return 1907;
}

iDataStandart* CreateSurfaceViewerGenMapDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new SurfaceViewerGenMapDataStandart(pack, base_folder, pService);
}