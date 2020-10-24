#pragma once
#include "AlgorithmBase.h"

namespace SV::chart_object
{
   class WaveFrontline : public AlgorithmBase
   {
   public:
      WaveFrontline(central_pack* pack, navigation_dispatcher::iComService* service)
         : AlgorithmBase(pack, service)
      {}

      chart_object_unit_vct generateIsolineLevel(const pathfinder::GeoMatrix& rawdata, double height, int H) override final;
   };
}