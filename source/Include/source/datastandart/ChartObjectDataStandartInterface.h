#pragma once
#include "DataStandart.h"
#include "DataStandartInterfaceBase.h"
#include "navdisp/ComService.h"

namespace data_standart
{
   struct iChartObjectDataStandart : public iDataStandart
   {
      // Common
      // Read
      // Write
      virtual void SaveData() = 0;
   };
}

#ifdef DATASTANDART_EXPORTS
#define CHARTOBJEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHARTOBJEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHARTOBJEXPRTIMPRT data_standart::iDataStandart * CreateChartObjectDataStandart(central_pack * pack, LPCWSTR base_folder, navigation_dispatcher::iComService * pService);