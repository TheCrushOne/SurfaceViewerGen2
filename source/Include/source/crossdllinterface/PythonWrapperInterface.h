#pragma once

#include "colreg/BaseRef.h"
#include "common/central_class.h"

namespace SV::python_wrapper
{
   struct iPythonWrapper : iReleasable
   {
      virtual void RunScript() = 0;
   };
}

#ifdef PYTHONMISCELLANEOUSWRAPPER_EXPORTS
#define PWEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define PWEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" PWEXPRTIMPRT SV::python_wrapper::iPythonWrapper * CreatePythonWrapper(SV::central_pack*);