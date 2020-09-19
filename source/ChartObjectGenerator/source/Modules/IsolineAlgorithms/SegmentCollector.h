#pragma once
#include "AlgorithmBase.h"

namespace chart_object
{
   class SegmentCollector : public AlgorithmBase
   {
   public:
      SegmentCollector(central_pack* pack, navigation_dispatcher::iComService* service)
         : AlgorithmBase(pack, service)
         , m_maxRadius(0)
      {}

      std::vector<geometry_chart_object> generateIsolineLevel(const pathfinder::GeoMatrix* rawdata, double height, int H) override final;
   private:
      double m_maxRadius;
   };
}