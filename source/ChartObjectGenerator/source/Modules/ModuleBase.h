#pragma once

#include "common/central_class.h"
#include "common/chart_object.h"
#include "GeneratorModuleInterface.h"
#include "common/converter_structs.h"

namespace chart_object
{
   class ModuleBase : public iGeneratorModule, public Central
   {
   protected:
      ModuleBase(central_pack* pack)
         : Central(pack)
      {}
   public:
      bool SetAdder(std::function<geometry_chart_object&()> crMtd, std::function<void(geometry_chart_object&)> addMtd) override final
      {
         m_addObject = addMtd;
         m_genNewObjectRef = crMtd;
         return true;
      }
   protected:
      std::function<void(geometry_chart_object&)> m_addObject;
      std::function<geometry_chart_object&()> m_genNewObjectRef;
   };
}