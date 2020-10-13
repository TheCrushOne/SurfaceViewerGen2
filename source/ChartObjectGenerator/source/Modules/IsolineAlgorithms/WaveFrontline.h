#pragma once
#include "AlgorithmBase.h"

namespace chart_object
{
   class WaveFrontline : public AlgorithmBase
   {
   public:
      WaveFrontline(central_pack_ptr pack, navigation_dispatcher::iComServicePtr service)
         : AlgorithmBase(pack, service)
      {}

      chart_object::chart_object_unit_vct generateIsolineLevel(const pathfinder::GeoMatrix& rawdata, double height, int H) override final;
   };
}