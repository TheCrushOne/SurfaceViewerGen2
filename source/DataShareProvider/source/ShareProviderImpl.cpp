#include "stdafx.h"
#include "ShareProviderImpl.h"

#include <iostream>
#include <fstream>

using namespace data_share;

//ShareProviderImpl::ShareProviderImpl() {}

void ShareProviderImpl::Share(share_meta& meta, const double** data)
{
   std::ofstream fil;
   fil.open(meta.shared_filename, std::ios::out | std::ios::binary);
   for (size_t i = 0; i < 300; i++)
   {
      for (size_t j = 0; j < 300; j++)
         fil << data[i][j] << " ";
      fil << "\n";
   }
   fil.close();
}

void ShareProviderImpl::GetShared(const share_meta& meta, double*** data)
{
   std::ifstream fil;
   fil.open(meta.shared_filename, std::ios::out | std::ios::binary);
   for (size_t i = 0; i < 300; i++)
   {
      for (size_t j = 0; j < 300; j++)
         fil >> (*data)[i][j];
   }
   fil.close();
}

data_share::iDataShareProvider* CreateDataShareProvider()
{
   return new ShareProviderImpl();
}