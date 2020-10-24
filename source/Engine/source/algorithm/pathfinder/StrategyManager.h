#pragma once

#include "common\pathfinder_structs.h"
#include "common/communicator.h"
#include "common/central_class.h"

namespace SV::pathfinder
{
   class StrategyManager : public Central
   {
   public:
      StrategyManager(central_pack* pack)
         : Central(pack)
      {}
      bool PrepareControlPoint(size_t iterations, std::vector<settings::route>& landRoutes, std::vector<settings::route>& airRoutes, const std::shared_ptr<pathfinder::RoutePointMatrix>& rawdata, const std::shared_ptr<pathfinder::path_finder_indata> indata);
   private:
      CG::route_point checkRetranslateFlyPointAffilation(int row, int col, const std::shared_ptr<pathfinder::RoutePointMatrix>& rawdata);
   };
}