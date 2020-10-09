#pragma once
#include "DataStandart.h"

namespace data_standart
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

extern "C" CHARTOBJEXPRTIMPRT data_standart::iDataStandart * CreatePackHoundDataStandart(central_pack * pack, LPCWSTR base_folder, navigation_dispatcher::iComService * pService);