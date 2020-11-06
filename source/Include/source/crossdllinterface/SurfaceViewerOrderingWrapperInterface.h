#pragma once

#include "common/central_class.h"
#include "colreg/BaseRef.h"

namespace SV::surface_ordering
{
   struct iOrderingWrapper : iReleasable
   {
      virtual bool ProcessOrder(const char* orderFile, const char* begCommand, std::unordered_map<std::string, std::string>& dict) = 0;
   };
}

#ifdef SURFACEVIEWERORDERINGWRAPPER_EXPORTS
#define SVORDWRAPEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SVORDWRAPEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SVORDWRAPEXPRTIMPRT SV::surface_ordering::iOrderingWrapper* CreateSurfaceViewerOrderingWrapper(SV::central_pack*, const char*);