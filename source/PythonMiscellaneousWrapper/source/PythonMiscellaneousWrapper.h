#pragma once

#include "crossdllinterface\PythonWrapperInterface.h"

namespace SV::python_wrapper
{
   class PythonMiscellaneousWrapper
      : public iPythonWrapper
      , public Central
   {
   public:
      PythonMiscellaneousWrapper(central_pack* pack);
      ~PythonMiscellaneousWrapper();
   public:
      void Release() override final { delete this; }
      void RunScript() override final;
   };
}