#include "stdafx.h"
#include "ShareProviderImpl.h"

#include <iostream>
#include <fstream>

using namespace data_share;

//ShareProviderImpl::ShareProviderImpl() {}

void ShareProviderImpl::Share(const wchar_t* fileName, const settings::map_settings& settings, const double** data)
{
   std::ofstream fil;
   fil.open(fileName, std::ios::out | std::ios::binary);
   for (size_t i = 0; i < settings.row_count; i++)
   {
      for (size_t j = 0; j < settings.col_count; j++)
         fil << data[i][j] << " ";
      fil << "\n";
   }
   fil.close();
}

void ShareProviderImpl::GetShared(const wchar_t* fileName, const settings::map_settings& settings, double*** data)
{
   std::ifstream fil;
   fil.open(fileName, std::ios::out | std::ios::binary);
   *data = new double* [settings.row_count];
   for (size_t i = 0; i < settings.row_count; i++)
   {
      (*data)[i] = new double[settings.col_count];
      for (size_t j = 0; j < settings.col_count; j++)
         fil >> (*data)[i][j];
   }
   fil.close();
}

data_share::iDataShareProvider* CreateDataShareProvider()
{
   return new ShareProviderImpl();
}