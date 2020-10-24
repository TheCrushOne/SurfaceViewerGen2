#pragma once

namespace SV::CG
{
   //! Polar coordinates point structure
   struct polar_point
   {
      double course;   ///< Course to the point, azimuth
      double distance; ///< Distance to the point, miles
   };
}