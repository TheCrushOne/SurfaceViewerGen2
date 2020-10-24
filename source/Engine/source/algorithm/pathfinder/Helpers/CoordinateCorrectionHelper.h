#pragma once

#include "common\pathfinder_structs.h"
#include "common/communicator.h"
#include "common/central_class.h"

namespace SV
{
   using affilationCheckerMtd = std::function<bool(const std::shared_ptr<pathfinder::RoutePointMatrix>&, size_t, size_t)>;

   class CoordinateCorrectionHelper
   {
   public:
      static SV::CG::route_point CorrectPoint(const std::shared_ptr<pathfinder::RoutePointMatrix>& rawdata, int row, int col, affilationCheckerMtd checker, ICommunicator* communicator);
   };
}