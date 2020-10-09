#pragma once

#include "common/central_class.h"

namespace python_wrapper
{
   struct iPythonWrapper : colreg::iReleasable
   {
      virtual void RunScript() = 0;
   };
}

#ifdef PYTHONMISCELLANEOUSWRAPPER_EXPORTS
#define PWEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define PWEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" PWEXPRTIMPRT python_wrapper::iPythonWrapper * CreatePythonWrapper(central_pack_ptr);