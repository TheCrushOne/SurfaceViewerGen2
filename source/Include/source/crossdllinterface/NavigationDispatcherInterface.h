#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/central_class.h"
#include "navdisp/ComService.h"

namespace navigation_dispatcher
{
   struct iNavigationDispatcher : colreg::iReleasable
   {
      virtual int ProcessCommand(const char* configFilePath, const char* begCommandName, const char* baseFolder, iComService* service = nullptr) = 0;
   };
}

#ifdef NAVDISP_DLL
#define NDEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define NDEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" NDEXPRTIMPRT navigation_dispatcher::iNavigationDispatcher * CreateNavigationDispatcher(central_pack*);
extern "C" NDEXPRTIMPRT navigation_dispatcher::iComService * CreateExternalComService(central_pack*, const char*);