#pragma once
#include "DataStandart.h"
#include "navdisp/ComService.h"
#include "common/pathfinder_structs.h"

namespace data_standart
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

extern "C" CHARTOBJEXPRTIMPRT data_standart::iDataStandart * CreatePathStorageDataStandart(central_pack * pack, LPCWSTR base_folder, navigation_dispatcher::iComService * pService);