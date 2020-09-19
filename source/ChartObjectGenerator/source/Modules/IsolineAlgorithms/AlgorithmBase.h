#pragma once
#include "common/converter_structs.h"
#include "common/central_class.h"
#include "common/chart_object.h"
#include "common/pathfinder_structs.h"
#include "math/math_utils.h"

namespace chart_object
{
   struct iAlgorithm
   {
      virtual std::vector<geometry_chart_object> GenerateIsolineLevel(const pathfinder::GeoMatrix* rawdata, double height, int H) = 0;
   protected:
      virtual std::vector<geometry_chart_object> generateIsolineLevel(const pathfinder::GeoMatrix* rawdata, double height, int H) = 0;
   };

   struct AlgorithmBase : public Central, public iAlgorithm
   {
      AlgorithmBase(central_pack* pack, std::function<void(const std::vector<math::geo_points>&, double, int)> adder)
         : Central(pack)
         , m_chartObjectSetAdder(adder)
      {}

      std::vector<geometry_chart_object> GenerateIsolineLevel(const pathfinder::GeoMatrix* rawdata, double height, int H) override final
      {
         return generateIsolineLevel(rawdata, height, H);
      }
   protected:
      std::function<void(const std::vector<math::geo_points>&, double, int)> m_chartObjectSetAdder;
   };
}