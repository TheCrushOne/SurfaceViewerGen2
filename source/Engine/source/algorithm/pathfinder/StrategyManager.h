#pragma once

#include "common\pathfinder_types.h"

class StrategyManager
{
public:
   bool PrepareControlPoint(pathfinder::strategy_settings settings, size_t iterations, std::vector<route>& landRoutes, std::vector<pathfinder::route>& airRoutes, const std::shared_ptr<pathfinder::Matrix<SVCG::route_point>>& rawdata);
};