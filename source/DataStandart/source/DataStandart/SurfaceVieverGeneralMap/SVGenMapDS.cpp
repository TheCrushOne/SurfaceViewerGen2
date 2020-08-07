#include "stdafx.h"
#include "SVGenMapDS.h"
#include <fstream>
#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace data_standart;

void SurfaceViewerGenMapDataStandart::resolvePathDee()
{

}

//void SurfaceViewerGenMapDataStandart::SetData(const pathfinder::GeoMatrix& rawData, const svgm_meta& meta)
//{
//   m_rawdata = TransitPtr<pathfinder::GeoMatrix>(rawData);
//   m_meta = meta;
//}
//
//void SurfaceViewerGenMapDataStandart::ReadFromSource()
//{
//   readMetaData();
//   readHeightData();
//}
//
//void SurfaceViewerGenMapDataStandart::WriteToDestination()
//{
//   saveMetaData();
//   saveHeightData();
//}

void SurfaceViewerGenMapDataStandart::readMetaData()
{
   std::wstring metaDataPath = getMetaFilePath();
   std::ifstream i(metaDataPath, std::ios_base::in | std::ios::binary);
   json j;
   i >> j;
   m_meta.row_count = j["row_count"].get<size_t>();
   m_meta.col_count = j["col_count"].get<size_t>();
}

void SurfaceViewerGenMapDataStandart::readHeightData()
{
   std::wstring dataFilePath = getDataFilePath();
   std::ifstream file(dataFilePath, std::ios::in | std::ios::binary);
   m_rawdata.Get()->SetRowCount(m_meta.row_count);
   m_rawdata.Get()->SetColCount(m_meta.col_count);
   char* buffer = new char[sizeof(double)];
   for (size_t ridx = 0; ridx < m_meta.row_count; ridx++)
   {
      for (size_t cidx = 0; cidx < m_meta.col_count; cidx++)
      {
         file.read(buffer, sizeof(double));
         m_rawdata.Get()->Set(ridx, cidx, *reinterpret_cast<double*>(buffer));
      }
   }
   delete [] buffer;
   file.close();
}

void SurfaceViewerGenMapDataStandart::saveMetaData()
{
   std::wstring metaDataPath = getMetaFilePath();
   std::ofstream o(metaDataPath);
   json j;
   j["row_count"] = m_meta.row_count;
   j["col_count"] = m_meta.col_count;
   o << j;
}

void SurfaceViewerGenMapDataStandart::saveHeightData()
{
   std::wstring dataFilePath = getDataFilePath();
   std::ofstream file(dataFilePath, std::ios::out | std::ios::binary);
   for (size_t ridx = 0; ridx < m_meta.row_count; ridx++)
   {
      for (size_t cidx = 0; cidx < m_meta.col_count; cidx++)
      {
         double buffer = m_rawdata.Get()->Get(ridx, cidx);
         file.write(reinterpret_cast<const char*>(&buffer), sizeof(double));
      }
   }
   file.close();
}

iDataStandart* CreateSurfaceViewerGenMapDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new SurfaceViewerGenMapDataStandart(pack, base_folder, pService);
}