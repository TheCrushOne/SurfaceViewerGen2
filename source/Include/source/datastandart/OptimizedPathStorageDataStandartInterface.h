#pragma once
#include "DataStandart.h"
#include "navdisp/ComService.h"
#include "PathStorageDataStandartInterface.h"

namespace data_standart
{
   struct iOptimizedPathStorageDataStandart : public iPathStorageDataStandart
   {
   };
}

#ifdef DATASTANDART_EXPORTS
#define CHARTOBJEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHARTOBJEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHARTOBJEXPRTIMPRT data_standart::iDataStandart * CreateOptimizedPathStorageDataStandart(central_pack * pack, LPCWSTR base_folder, navigation_dispatcher::iComService * pService);