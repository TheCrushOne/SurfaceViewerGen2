#pragma once

#include "common/central_class.h"
#include "chart_storage.h"
#include "ModuleBase.h"
#include "math/math_utils.h"

namespace chart_object
{
   enum class AlgorithmType : unsigned short
   {
      AT_RC1 = 0,
      AT_RC2,
   };

   enum class TraversalDirection : unsigned short
   {
      TD_U = 0,
      TD_R,
      TD_LW,
      TD_L,
   };

   const static std::unordered_map<TraversalDirection, TraversalDirection> nextStepRule = {
      { TraversalDirection::TD_U, TraversalDirection::TD_R },
      { TraversalDirection::TD_R, TraversalDirection::TD_LW },
      { TraversalDirection::TD_LW, TraversalDirection::TD_L },
      { TraversalDirection::TD_L, TraversalDirection::TD_U },
   };

   // NOTE: вообще тут просто свап, но его в стл нету...так что пока так, хардкодом...
   const static std::unordered_map<TraversalDirection, TraversalDirection> nextPointPickRule = {
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

   class IsolineGenerator : public ModuleBase
   {
   public:
      IsolineGenerator() {}
      void GenerateIsolines(const converter::raw_data_ref& rawdata);
   private:
      void generateIsolineLevel(AlgorithmType type, const converter::raw_data_ref& rawdata, double height, int H);
      void generateRC1(const converter::raw_data_ref& rawdata, double height, int H);
      void generateRC2(const converter::raw_data_ref& rawdata, double height, int H);

      void addChartObjectSet(const std::vector<math::geo_points>& data, double height, int H);
   private:
      double m_maxRadius;
   };
}