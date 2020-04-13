#pragma once

#include "colreg/BaseRef.h"

namespace converter
{
   using data_line_ref = colreg::base_ref<double>;
   using raw_data_ref = colreg::base_ref<data_line_ref>;
}