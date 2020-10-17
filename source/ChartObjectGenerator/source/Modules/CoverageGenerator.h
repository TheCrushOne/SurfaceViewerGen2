#pragma once

#include "common/central_class.h"
#include "common/pathfinder_structs.h"
#include "common/chart_object.h"
#include "ModuleBase.h"

namespace SV::chart_object
{
   class CoverageGenerator : public ModuleBase
   {
   public:
      CoverageGenerator(central_pack_ptr pack, navigation_dispatcher::iComServicePtr service)
         : ModuleBase(pack, service)
      {}

      void GenerateChartBorder(const pathfinder::GeoMatrix&, chart_object_unit&);
   private:
   };
}