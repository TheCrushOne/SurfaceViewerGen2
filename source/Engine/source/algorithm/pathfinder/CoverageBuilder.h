#pragma once

#include <memory>
#include "common\settings.h"
#include "common\pathfinder_types.h"

class CoverageBuilder
{
public:
   CoverageBuilder() {}

   std::shared_ptr<pathfinder::Matrix<size_t>> BuildLandCoverage(size_t rowCount, size_t colCount, pathfinder::strategy_settings settings, std::vector<pathfinder::route>& airRoutes);
   bool LandPathCoverage(size_t rowCount, size_t colCount, pathfinder::strategy_settings settings, std::vector<pathfinder::route>& landRoutes, std::vector<pathfinder::route>& airRoutes);
private:
};