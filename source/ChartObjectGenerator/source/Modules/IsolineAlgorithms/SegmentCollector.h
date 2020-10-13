#pragma once
#include "AlgorithmBase.h"

namespace chart_object
{
   class SegmentCollector : public AlgorithmBase
   {
   public:
      SegmentCollector(central_pack_ptr pack, navigation_dispatcher::iComServicePtr service)
         : AlgorithmBase(pack, service)
         , m_maxRadius(0)
      {}

      chart_object::chart_object_unit_vct generateIsolineLevel(const pathfinder::GeoMatrix& rawdata, double height, int H) override final;
   private:
      double m_maxRadius;
   };
}