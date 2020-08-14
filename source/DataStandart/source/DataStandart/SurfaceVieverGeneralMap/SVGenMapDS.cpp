#include "stdafx.h"
#include "SVGenMapDS.h"
#include <fstream>
#include <nlohmann/json.hpp>

#include <filesystem>

using namespace nlohmann;
using namespace data_standart;

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

iDataStandart* CreateSurfaceViewerGenMapDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new SurfaceViewerGenMapDataStandart(pack, base_folder, pService);
}