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
   using chart_context_ref = base_ref<chart_context>;

   struct track_full_info_ref
   {
      track_full_info_ref() = default;

      track_full_info_ref(const track_point_info* pts,
                          const model_point_info* modelInfo,
                          const chart_context* chartContext,
                          const domain_border_info* domainBorder,
                          size_t sz)
         : points{ pts }
         , model_info{ modelInfo }
         , chart_context{ chartContext }
         , domain_border{ domainBorder }
         , size{ sz }
      {}

      const track_point_info*   points;
      const model_point_info*   model_info;
      const chart_context*      chart_context;
      const domain_border_info* domain_border;
      size_t size;
   };

#pragma pack (pop)
} //namespace colreg