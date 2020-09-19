#pragma once
#include "AlgorithmBase.h"

namespace chart_object
{
   class SegmentCollector : public AlgorithmBase
   {
   public:
      SegmentCollector(central_pack* pack, std::function<void(const std::vector<math::geo_points>&, double, int)> adder)
         : AlgorithmBase(pack, adder)
      {}

      std::vector<geometry_chart_object> generateIsolineLevel(const pathfinder::GeoMatrix* rawdata, double height, int H) override final;
   private:
      double m_maxRadius;
   };
}