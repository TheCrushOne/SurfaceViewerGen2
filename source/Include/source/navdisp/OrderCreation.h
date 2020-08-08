#pragma once

#include "OrderInterface.h"
#include "common/central_class.h"
#include "navdisp/ComService.h"

#ifdef ORDER_EXPORTS
#define ORDER_EXPORTIMPORT __declspec(dllexport) // export DLL information
#else
#define ORDER_EXPORTIMPORT __declspec(dllimport) // import DLL information
#endif

#define PREPARE_ORDER(name) extern "C" ORDER_EXPORTIMPORT navigation_dispatcher::iOrder* name(central_pack* pack, navigation_dispatcher::iComService* pService);

PREPARE_ORDER(CreatePngHeightmapConverter);
PREPARE_ORDER(CreateObjectListGenerator);
PREPARE_ORDER(CreatePathfinder);
PREPARE_ORDER(CreateOptimizedPathfinder);
PREPARE_ORDER(CreatePackHound);