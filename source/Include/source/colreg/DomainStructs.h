#pragma once
#include "CommonStructs.h"
#include "SVCG/base_geometry.h"

namespace colreg
{
   enum class DOGT_POLYGON_PART
   {
      DOGT_POLYGON_POINT,
      DOGT_POLYGON_SIZE
   };

   enum class DOGT_SECTOR_PART : unsigned char
   {
      DOGT_SECTOR_CENTER,
      DOGT_SECTOR_BEGIN,
      DOGT_SECTOR_END,
      DOGT_SECTOR_SIZE
   };

   enum class DOGT_ARC_PART : unsigned char
   {
      DOGT_ARC_CENTER = 0,
      DOGT_ARC_MAJOR,
      DOGT_ARC_MINOR,
      DOGT_ARC_BEGIN,
      DOGT_ARC_END,
      DOGT_ARC_SIZE
   };

   enum class DOMAIN_GEOMETRY_TYPE : char
   {
      DOGT_POLILYNE = 0, /* Each 2 points form a line - from first to the second to the third to the forth etc. */
      DOGT_SECTOR = 1,   /* Each 3 points form an arc - first is the center
                                                        second is the arc beginning
                                                        third is the arc end.
                            Arc length is always nonzero and the default sweep direction is CW. */
      DOGT_ARC = 2       /* Each 4 points form an elliptic arc - first is the center
                                                                 second is the ellipse X-axis radius point
                                                                 third is the ellipse Y-axis radius point
                                                                 fourth point is the start angle point
                                                                 fifth point is the end angle point.
                            Arc length is always nonzero and the default sweep direction is CW. */
   };

#pragma pack (push, 1)

   struct ship_danger_estimation
   {
      bool   is_danger;           ///< Is in dangerous situation
      bool   is_danger_near_turn; ///< Is in dangerous situation near a circulation
      double distance;            ///< Distance to the target
   };

   struct domain_border_info
   {
      domain_border_info() = default;

      double    width;
      SVCG::geo_point left;
      SVCG::geo_point right;
   };

#pragma pack (pop)
}