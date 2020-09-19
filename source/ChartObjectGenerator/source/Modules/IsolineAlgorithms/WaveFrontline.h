#pragma once
#include "AlgorithmBase.h"

namespace chart_object
{
   class WaveFrontline : public AlgorithmBase
   {
   public:
      WaveFrontline(central_pack* pack, navigation_dispatcher::iComService* service)
         : AlgorithmBase(pack, service)
      {}

      std::vector<geometry_chart_object> generateIsolineLevel(const pathfinder::GeoMatrix* rawdata, double height, int H) override final;
   };
}