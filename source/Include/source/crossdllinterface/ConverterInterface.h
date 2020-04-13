#pragma once

#include "SurfaceInterfaces.h"
#include "common/file_storage.h"
#include "common/communicator.h"
#include "common/central_class.h"

namespace converter
{
   struct iConverter : colreg::iReleasable
   {
      virtual void Init(central_pack*) = 0;
      virtual bool Convert() = 0;
   };
}

#ifdef CONVERTER_DLL
#define CONVEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CONVEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CONVEXPRTIMPRT converter::iConverter * CreateConverter();