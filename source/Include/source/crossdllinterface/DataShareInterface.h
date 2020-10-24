#pragma once

#include "SurfaceInterfaces.h"
#include "common/central_class.h"

namespace SV::data_share
{
   struct iDataShareProvider : iReleasable
   {
      virtual void Share(const wchar_t*, const std::vector<std::vector<double>>&) = 0;
      virtual void GetShared(const wchar_t*, std::vector<std::vector<double>>&) = 0;
   };
}

#ifdef DATASHARE_DLL
#define DSHAREEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define DSHAREEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" DSHAREEXPRTIMPRT SV::data_share::iDataShareProvider * CreateDataShareProvider(SV::central_pack*);