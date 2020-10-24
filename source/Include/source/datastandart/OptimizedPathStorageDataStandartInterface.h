#pragma once
#include "DataStandart.h"
#include "navdisp/ComService.h"
#include "PathStorageDataStandartInterface.h"

namespace SV::data_standart
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

extern "C" CHARTOBJEXPRTIMPRT SV::data_standart::iDataStandart * CreateOptimizedPathStorageDataStandart(SV::central_pack * pack, LPCWSTR base_folder, SV::navigation_dispatcher::iComService * pService);