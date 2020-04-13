#pragma once

#include "common/central_class.h"
#include "chart_storage.h"
#include "GeneratorModuleInterface.h"
#include "common/converter_structs.h"

namespace chart_object
{
   class ModuleBase : public iGeneratorModule, public Central
   {
   public:
      ModuleBase() = default;
      bool SetAdder(std::function<chart_storage&()> crMtd, std::function<void(chart_storage&)> addMtd) override final
      {
         m_addObject = addMtd;
         m_genNewObjectRef = crMtd;
         return true;
      }
   protected:
      std::function<void(chart_storage&)> m_addObject;
      std::function<chart_storage&()> m_genNewObjectRef;
   };
}