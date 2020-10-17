#pragma once

#include <vector>
#include <functional>
#include <memory>

namespace SV::pathfinder
{
   enum class FlyZoneAffilation : unsigned short
   {
      FZA_NORMAL,
      FZA_DANGEROUS,
      FZA_FORBIDDEN,
   };
   enum class GoZoneAffilation : unsigned short
   {
      GZA_NORMAL,
      GZA_DANGEROUS,
      GZA_FORBIDDEN,
   };

   enum StrategyType : unsigned short
   {
      ST_RHOMBOID = 0,
      ST_SECTOR,
      ST_SNAKE
   };

   using height_corrector = std::function<float(float)>;
}