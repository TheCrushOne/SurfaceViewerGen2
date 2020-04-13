#pragma once

#include "BaseRef.h"
#include "CommonStructs.h"
#include "ModelStructs.h"
#include "DomainStructs.h"
#include "ChartSafetyStructs.h"

namespace colreg
{
#pragma pack (push, 1)

   using route_ref = base_ref<route_point>;
   using track_ref = base_ref<track_point_info>;

   struct track_full_info_ref
   {
      track_full_info_ref() = default;

      track_full_info_ref(const track_point_info* pts,
                          const model_point_info* modelInfo,
                          const domain_border_info* domainBorder,
                          size_t sz)
         : points{ pts }
         , model_info{ modelInfo }
         , domain_border{ domainBorder }
         , size{ sz }
      {}

      const track_point_info*   points;
      const model_point_info*   model_info;
      const domain_border_info* domain_border;
      size_t size;
   };

#pragma pack (pop)
} //namespace colreg