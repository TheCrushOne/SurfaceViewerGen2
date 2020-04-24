#pragma once
#include "common/converter_structs.h"
#include "common/central_class.h"
#include "math/math_utils.h"

namespace chart_object
{
   struct iAlgorithm : public Central
   {
      iAlgorithm(std::function<void(const std::vector<math::geo_points>&, double, int)> adder)
         : m_chartObjectSetAdder(adder)
      {}

      virtual void GenerateIsolineLevel(const converter::raw_data_ref& rawdata, double height, int H) = 0;
   protected:
      std::function<void(const std::vector<math::geo_points>&, double, int)> m_chartObjectSetAdder;
   };
}