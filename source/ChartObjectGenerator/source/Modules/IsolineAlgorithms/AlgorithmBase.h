#pragma once
#include "common/central_class.h"
#include "common/servicable.h"
#include "common/chart_object_unit.h"
#include "common/pathfinder_structs.h"
#include "math/math_utils.h"

namespace SV::chart_object
{
   struct iAlgorithm
   {
      virtual chart_object::chart_object_unit_vct GenerateIsolineLevel(const pathfinder::GeoMatrix& rawdata, double height, int H) = 0;
   protected:
      virtual chart_object::chart_object_unit_vct generateIsolineLevel(const pathfinder::GeoMatrix& rawdata, double height, int H) = 0;
   };

   struct AlgorithmBase : public Central, public Servicable, public iAlgorithm
   {
      AlgorithmBase(central_pack* pack, navigation_dispatcher::iComService* service)
         : Central(pack)
         , Servicable(service)
      {}

      chart_object::chart_object_unit_vct GenerateIsolineLevel(const pathfinder::GeoMatrix& rawdata, double height, int H) override final
      {
         return generateIsolineLevel(rawdata, height, H);
      }
   protected:

   };
}