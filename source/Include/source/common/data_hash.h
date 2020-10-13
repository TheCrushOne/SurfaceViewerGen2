#pragma once

#include <limits>

namespace data_hash
{
   constexpr size_t INVALID_HASH = std::numeric_limits<size_t>::max();

   struct hash_junction
   {
      size_t source;
      size_t destination;
   };
}