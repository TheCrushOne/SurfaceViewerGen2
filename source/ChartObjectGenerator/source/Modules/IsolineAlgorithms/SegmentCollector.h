#pragma once
#include "AlgorithmBase.h"

namespace SV::chart_object
{
   class SegmentCollector : public AlgorithmBase
   {
   public:
      SegmentCollector(central_pack* pack, navigation_dispatcher::iComService* service)
         : AlgorithmBase(pack, service)
         , m_maxRadius(0)
      {}

      chart_object::chart_object_unit_vct generateIsolineLevel(const pathfinder::GeoMatrix& rawdata, double height, int H) override final;
   private:
      double m_maxRadius;
   };
}