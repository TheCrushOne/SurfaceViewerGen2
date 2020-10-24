#pragma once

#include "common/header_collector.h"
#include "crossdllinterface/UnitDataSerializerInterface.h"

namespace SV::serializer
{
   struct tag
   {
      static constexpr char name[] = "name";

      static constexpr char start[] = "start";
      static constexpr char finish[] = "finish";

      static constexpr char row[] = "row";
      static constexpr char col[] = "col";

      static constexpr char control_points[] = "control_points";

      static constexpr char land_units[] = "land_units";
      static constexpr char air_units[] = "air_units";
   };

   struct iJsonUnitDataSerializer : iUnitDataSerializer
   {};
}