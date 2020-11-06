#pragma once
#include "DataStandart.h"

namespace SV::data_standart
{
   class iPackHoundConfigDataStandart : public iDataStandart
   {
      // Common
      // Read
      // Write
   };
}

#ifdef DATASTANDART_EXPORTS
#define CHARTOBJEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHARTOBJEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHARTOBJEXPRTIMPRT SV::data_standart::iDataStandart * CreatePackHoundDataStandart(SV::central_pack * pack, LPCSTR base_folder, SV::navigation_dispatcher::iComService * pService);