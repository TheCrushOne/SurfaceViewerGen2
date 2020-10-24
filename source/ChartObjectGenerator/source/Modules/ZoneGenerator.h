#pragma once

#include "common/central_class.h"
#include "common/chart_object_unit.h"
#include "ModuleBase.h"

namespace SV::chart_object
{
   class ZoneGenerator : public ModuleBase
   {
   public:
      ZoneGenerator(central_pack *pack, navigation_dispatcher::iComService* service)
         : ModuleBase(pack, service)
      {}

      void GenerateNoGoAreas();
      void GenerateNoFlyAreas();
   private:
   };
}