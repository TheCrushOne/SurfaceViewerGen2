#pragma once

#include "SurfaceInterfaces.h"
#include "common/central_class.h"

namespace surface_ordering
{
   struct iOrderingWrapper : colreg::iReleasable
   {
      virtual bool ProcessOrder(const wchar_t* orderFile, const wchar_t* begCommand, std::unordered_map<std::string, std::wstring>& dict) = 0;
   };
}

#ifdef SURFACEVIEWERORDERINGWRAPPER_EXPORTS
#define SVORDWRAPEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SVORDWRAPEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SVORDWRAPEXPRTIMPRT surface_ordering::iOrderingWrapper * CreateSurfaceViewerOrderingWrapper(central_pack_ptr, const wchar_t*);