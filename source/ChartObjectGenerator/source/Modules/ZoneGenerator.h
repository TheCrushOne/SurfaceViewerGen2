#pragma once

#include "common/central_class.h"
#include "common/chart_object.h"
#include "ModuleBase.h"

namespace chart_object
{
   class ZoneGenerator : public ModuleBase
   {
   public:
      ZoneGenerator(central_pack *pack, navigation_dispatcher::iComService* service)
         : ModuleBase(pack, service)
      {}

      void GenerateNoGoAreas(const converter::raw_data_ref& rawdata);
      void GenerateNoFlyAreas(const converter::raw_data_ref& rawdata);
   private:
   };
}