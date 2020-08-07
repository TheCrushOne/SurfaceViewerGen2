#pragma once

#include "OrderInterface.h"

#ifdef ORDER_EXPORTS
#define ORDER_EXPORTIMPORT __declspec(dllexport) // export DLL information
#else
#define ORDER_EXPORTIMPORT __declspec(dllimport) // import DLL information
#endif

#define PREPARE_ORDER(name) extern ORDER_EXPORTIMPORT navigation_dispatcher::iOrder* name(central_pack* pack, iComService* pService);

namespace navigation_dispatcher
{
   PREPARE_ORDER(CreatePngHeightmapConverter);
   PREPARE_ORDER(CreateObjectListGenerator);
   PREPARE_ORDER(CreatePathfinder);
   PREPARE_ORDER(CreateOptimizedPathfinder);
   PREPARE_ORDER(CreatePackHound);
}