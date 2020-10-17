#pragma once
#include "header_collector.h"

namespace ColregSimulation
{
   struct scenario_data
   {
      settings::unit_source_data unit_data;
      size_t step_count;
      size_t current_step;
   };
}