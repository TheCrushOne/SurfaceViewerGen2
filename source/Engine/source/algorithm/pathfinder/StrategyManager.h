#pragma once

#include "common\pathfinder_structs.h"

class StrategyManager
{
public:
   bool PrepareControlPoint(pathfinder::strategy_settings settings, size_t iterations, std::vector<pathfinder::route>& landRoutes, std::vector<pathfinder::route>& airRoutes, const std::shared_ptr<pathfinder::Matrix<SVCG::route_point>>& rawdata);
private:
   SVCG::route_point checkRetranslateFlyPointAffilation(int row, int col, const std::shared_ptr<pathfinder::Matrix<SVCG::route_point>>& rawdata);
};