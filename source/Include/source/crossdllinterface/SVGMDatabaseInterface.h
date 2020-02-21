#pragma once

namespace database
{
   struct chart_meta
   {
      unsigned int width;  // lat
      unsigned int length; // lon
      unsigned int height; // alt(optional)
   };

   struct iSVGMDatabaseController
   {
      virtual void SaveExternalData(const char* dstPath, const double** coordGrid, const chart_meta& meta) = 0;
   };
}