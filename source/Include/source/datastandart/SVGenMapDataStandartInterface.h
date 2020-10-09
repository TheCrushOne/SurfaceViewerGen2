#pragma once
#include "DataStandart.h"
#include "common/pathfinder_structs.h"
#include "navdisp/ComService.h"

namespace data_standart
{
   struct iSurfaceVieverGenMapDataStandart : public iDataStandart
   {
      virtual void SetData(const pathfinder::GeoMatrix&) = 0;
      virtual pathfinder::GeoMatrix& GetData() = 0;
      virtual settings::unit_source_data& GetUnitData() = 0;
   };
}

#ifdef DATASTANDART_EXPORTS
#define CHARTOBJEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHARTOBJEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHARTOBJEXPRTIMPRT data_standart::iDataStandart * CreateSurfaceViewerGenMapDataStandart(central_pack * pack, LPCWSTR base_folder, navigation_dispatcher::iComService * pService);