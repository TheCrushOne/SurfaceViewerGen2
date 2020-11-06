#pragma once
#include "DataStandart.h"
#include "navdisp/ComService.h"
#include "common/pathfinder_structs.h"

namespace SV::data_standart
{
   struct iPathStorageDataStandart : public iDataStandart
   {
      virtual void SetData(const pathfinder::route_data& paths) = 0;
      virtual const pathfinder::route_data& GetData() = 0;
   };
}

#ifdef DATASTANDART_EXPORTS
#define CHARTOBJEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHARTOBJEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHARTOBJEXPRTIMPRT SV::data_standart::iDataStandart * CreatePathStorageDataStandart(SV::central_pack * pack, LPCSTR base_folder, SV::navigation_dispatcher::iComService * pService);