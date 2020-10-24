#pragma once
#include "AlgorithmBase.h"

namespace SV::chart_object
{
   enum class TraversalDirection : unsigned short
   {
      TD_U = 0,
      TD_R,
      TD_LW,
      TD_L,
   };

   const static std::unordered_map<TraversalDirection, TraversalDirection> nextStepTransitionRule = {
      { TraversalDirection::TD_U, TraversalDirection::TD_R },
      { TraversalDirection::TD_R, TraversalDirection::TD_LW },
      { TraversalDirection::TD_LW, TraversalDirection::TD_L },
      { TraversalDirection::TD_L, TraversalDirection::TD_U },
   };

   // NOTE: вообще тут просто свап, но его в стл нету...так что пока так, хардкодом...
   const static std::unordered_map<TraversalDirection, TraversalDirection> notFoundTransitionRule = {
      { TraversalDirection::TD_R, TraversalDirection::TD_U },
      { TraversalDirection::TD_LW, TraversalDirection::TD_R },
      { TraversalDirection::TD_L, TraversalDirection::TD_LW },
      { TraversalDirection::TD_U, TraversalDirection::TD_L },
   };

   const static std::unordered_map<TraversalDirection, std::pair<std::pair<int, int>, std::pair<int, int>>> traversalOffset = {
      { TraversalDirection::TD_R, {{1, 1}, {0, 1}} },
      { TraversalDirection::TD_LW, {{-1, 1}, {-1, 0}} },
      { TraversalDirection::TD_L, {{-1, -1}, {0, -1}} },
      { TraversalDirection::TD_U, {{1, -1}, {1, 0}} },
   };

   class LabirinthTraverse : public AlgorithmBase
   {
   public:
      LabirinthTraverse(central_pack* pack, navigation_dispatcher::iComService* service)
         : AlgorithmBase(pack, service)
      {}

      chart_object_unit_vct generateIsolineLevel(const pathfinder::GeoMatrix& rawdata, double height, int H) override final;
   };
}