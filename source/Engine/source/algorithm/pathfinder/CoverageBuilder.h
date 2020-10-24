#pragma once

#include <memory>
#include "common\header_collector.h"
#include "common\pathfinder_structs.h"

namespace SV::pathfinder
{
   class CoverageBuilder
   {
   public:
      CoverageBuilder() {}

      pathfinder::SharedUnsignedMatrix BuildLandCoverage(size_t rowCount, size_t colCount, pathfinder::strategy_settings settings, std::vector<settings::route>& airRoutes);
      bool LandPathCoverage(size_t rowCount, size_t colCount, pathfinder::strategy_settings settings, std::vector<settings::route>& landRoutes, std::vector<settings::route>& airRoutes);
   private:
   };
}