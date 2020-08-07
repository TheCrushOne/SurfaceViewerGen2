#pragma once
#include "DataStandart.h"
#include "common/pathfinder_structs.h"
#include "navdisp/ComService.h"

namespace data_standart
{
   struct svgm_meta
   {
      size_t row_count;
      size_t col_count;
   };

   struct iSurfaceVieverGenMapDataStandart
   {
      //virtual void SetData(const pathfinder::GeoMatrix&, const svgm_meta&) = 0;
      //virtual pathfinder::GeoMatrix* GetData() = 0;
   };
}

#ifdef DATASTANDART_EXPORTS
#define CHARTOBJEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHARTOBJEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHARTOBJEXPRTIMPRT data_standart::iDataStandart * CreateSurfaceViewerGenMapDataStandart(central_pack * pack, LPCWSTR base_folder, navigation_dispatcher::iComService * pService);