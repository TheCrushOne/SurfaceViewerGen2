#pragma once

#include "SurfaceInterfaces.h"
#include "common/file_storage.h"

namespace converter
{
   struct iConverter : colreg::iReleasable
   {
      virtual bool Convert(const file_utils::file_storage_base& src, const file_utils::file_storage_base& dst) = 0;
   };
}

#ifdef CONVERTER_DLL
#define CONVEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CONVEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CONVEXPRTIMPRT converter::iConverter * CreateConverter();