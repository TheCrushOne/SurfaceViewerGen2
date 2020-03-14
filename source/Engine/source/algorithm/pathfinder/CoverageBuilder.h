#pragma once

#include <memory>
#include "common\settings.h"
#include "common\pathfinder_structs.h"

class CoverageBuilder
{
public:
   CoverageBuilder() {}

   std::shared_ptr<pathfinder::Matrix<size_t>> BuildLandCoverage(size_t rowCount, size_t colCount, pathfinder::strategy_settings settings, std::vector<settings::route>& airRoutes);
   bool LandPathCoverage(size_t rowCount, size_t colCount, pathfinder::strategy_settings settings, std::vector<settings::route>& landRoutes, std::vector<settings::route>& airRoutes);
private:
};