#pragma once

namespace surface_viewer
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

   struct check_fly_zone_result
   {
      FlyZoneAffilation fza;
   };

   struct check_go_zone_result
   {
      GoZoneAffilation gza;
      double asn, awe;   // angle south-north, angle west-east
   };
}