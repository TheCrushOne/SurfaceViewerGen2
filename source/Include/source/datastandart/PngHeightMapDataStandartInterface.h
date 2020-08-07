#pragma once
#include "DataStandart.h"
#include "navdisp/ComService.h"

namespace data_standart
{
   struct iPngHeightMapDataStandart
   {
      // Common
      virtual FILE* GetPngData() const = 0;
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

extern "C" PNGHMEXPRTIMPRT data_standart::iDataStandart * CreatePngHeighMapDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService);