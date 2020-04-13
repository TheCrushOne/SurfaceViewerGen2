#pragma once

#include "common\pathfinder_structs.h"
#include "common/communicator.h"
#include "common/central_class.h"

using affilationCheckerMtd = std::function<bool(const std::shared_ptr<pathfinder::Matrix<SVCG::route_point>>&, size_t, size_t)>;

class CoordinateCorrectionHelper
{
public:
   static SVCG::route_point CorrectPoint(const std::shared_ptr<pathfinder::Matrix<SVCG::route_point>>& rawdata, int row, int col, affilationCheckerMtd checker, ICommunicator* communicator);
};