#pragma once
#include "common/converter_structs.h"
#include "common/central_class.h"
#include "common/pathfinder_structs.h"
#include "math/math_utils.h"

namespace chart_object
{
   struct iAlgorithm
   {
      virtual void GenerateIsolineLevel(const pathfinder::GeoMatrix* rawdata, double height, int H) = 0;
   };

   struct AlgorithmBase : public Central, public iAlgorithm
   {
      AlgorithmBase(central_pack* pack, std::function<void(const std::vector<math::geo_points>&, double, int)> adder)
         : Central(pack)
         , m_chartObjectSetAdder(adder)
      {}
   protected:
      std::function<void(const std::vector<math::geo_points>&, double, int)> m_chartObjectSetAdder;
   };
}