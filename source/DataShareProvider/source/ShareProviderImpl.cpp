#include "stdafx.h"
#include "ShareProviderImpl.h"

#include <iostream>
#include <fstream>

using namespace data_share;

//ShareProviderImpl::ShareProviderImpl() {}

void ShareProviderImpl::Share(const wchar_t* fileName, const std::vector<std::vector<double>>& data)
{
   auto mapstt = GetSettings()->map_stt;
   std::ofstream fil;
   fil.open(fileName, std::ios::out | std::ios::binary);
   for (size_t i = 0; i < mapstt.row_count; i++)
   {
      for (size_t j = 0; j < mapstt.col_count; j++)
         fil << data.at(i).at(j) << " ";
      fil << "\n";
   }
   fil.close();
}

void ShareProviderImpl::GetShared(const wchar_t* fileName, std::vector<std::vector<double>>& data)
{
   auto mapstt = GetSettings()->map_stt;
   std::ifstream fil;
   fil.open(fileName, std::ios::out | std::ios::binary);
   data.resize(mapstt.row_count);
   for (size_t i = 0; i < mapstt.row_count; i++)
   {
      data[i].resize(mapstt.col_count);
      for (size_t j = 0; j < mapstt.col_count; j++)
         fil >> data[i][j];
   }
   fil.close();
}

data_share::iDataShareProvider* CreateDataShareProvider()
{
   return new ShareProviderImpl();
}