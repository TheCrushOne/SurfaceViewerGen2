#pragma once
#include "AlgorithmBase.h"

namespace chart_object
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

   // NOTE: ������ ��� ������ ����, �� ��� � ��� ����...��� ��� ���� ���, ���������...
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

   class LabirinthTraverse : public iAlgorithm
   {
   public:
      LabirinthTraverse(std::function<void(const std::vector<math::geo_points>&, double, int)> adder)
         : iAlgorithm(adder)
      {}

      void GenerateIsolineLevel(const converter::raw_data_ref& rawdata, double height, int H) override final;
   };
}