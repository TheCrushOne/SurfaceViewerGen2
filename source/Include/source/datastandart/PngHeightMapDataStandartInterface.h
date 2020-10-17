#pragma once
#include "DataStandart.h"
#include "navdisp/ComService.h"

namespace SV::data_standart
{
   typedef std::vector<std::vector<double>> png_matrix_data;

   struct png_data
   {
      size_t row_count, col_count;
      png_matrix_data raw_data;
   };

   struct iPngHeightMapDataStandart : public iDataStandart
   {
      // Common
      virtual const png_data& GetPngData() const = 0;
      // Read
      virtual void ReleasePngData() = 0;
      // Write
   };
}

#ifdef DATASTANDART_EXPORTS
#define PNGHMEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define PNGHMEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" PNGHMEXPRTIMPRT SV::data_standart::iDataStandart * CreatePngHeighMapDataStandart(central_pack* pack, LPCWSTR base_folder, SV::navigation_dispatcher::iComService* pService);