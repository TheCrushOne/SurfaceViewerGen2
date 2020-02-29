#pragma once

#include "SurfaceInterfaces.h"

namespace data_share
{
   struct iDataShareProvider : colreg::iReleasable
   {
      virtual void Share(const wchar_t*, const settings::map_settings&, const double**) = 0;
      virtual void GetShared(const wchar_t*, const settings::map_settings&, double***) = 0;
   };
}

#ifdef DATASHARE_DLL
#define DSHAREEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define DSHAREEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" DSHAREEXPRTIMPRT data_share::iDataShareProvider * CreateDataShareProvider();