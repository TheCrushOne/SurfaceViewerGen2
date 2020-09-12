#pragma once

#include "common/central_class.h"
#include "common/pathfinder_structs.h"
#include "comnon/chart_object.h"
#include "ModuleBase.h"

namespace chart_object
{
   class CoverageGenerator : public ModuleBase
   {
   public:
      CoverageGenerator(central_pack* pack)
         : ModuleBase(pack)
      {}

      void GenerateChartBorder(const pathfinder::GeoMatrix*, geometry_chart_object&);
   private:
   };
}