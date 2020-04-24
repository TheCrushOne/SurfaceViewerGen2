#pragma once
#include "AlgorithmBase.h"

namespace chart_object
{
   class SegmentCollector : public iAlgorithm
   {
   public:
      SegmentCollector(std::function<void(const std::vector<math::geo_points>&, double, int)> adder)
         : iAlgorithm(adder)
      {}

      void GenerateIsolineLevel(const converter::raw_data_ref& rawdata, double height, int H) override final;
   private:
      double m_maxRadius;
   };
}