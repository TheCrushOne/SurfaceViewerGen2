#pragma once

#include <limits>
#include <string>
#include <vector>

namespace SV
{
   using id_type = unsigned long long int;
   using mask_type = long long int;
   using chart_object_id = unsigned long long;

   constexpr double NO_VALUE = std::numeric_limits<double>::max();
   constexpr double INVALID_TIME = std::numeric_limits<double>::max();

   constexpr size_t  MAX_SHIP_CALL_LENGHT = 32;
   constexpr size_t  MAX_SHIP_NAME_LENGHT = 128;
   constexpr size_t  INVALID_INDEX = std::numeric_limits<size_t>::max();
   constexpr id_type INVALID_ID = std::numeric_limits<id_type>::max();
   constexpr int INVALID_INT_ID = std::numeric_limits<int>::max();
}