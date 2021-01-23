#pragma once

#include "colreg/BaseRef.h"
#include "crossdllinterface\SettingsSerializerInterface.h"

#include "json/json_wrapper.h"

namespace SV::serializer
{
   struct iJsonSettingsSerializer : iSettingsSerializer
   {
      struct tag
      {
         static constexpr char max[] = "max";
         static constexpr char min[] = "min";
         static constexpr char step[] = "step";
         static constexpr char values[] = "values";

         static constexpr char dangerous_land_angle[] = "dangerous_land_angle";
         static constexpr char max_air_height[] = "max_air_height";
         static constexpr char max_land_angle[] = "max_land_angle";
         static constexpr char max_land_height[] = "max_land_height";
         static constexpr char min_land_height[] = "min_land_height";
         
         static constexpr char level_settings[] = "level_settings";

         static constexpr char debug_level[] = "debug_level";
         static constexpr char fly_count_range[] = "fly_count_range";
         static constexpr char iter_count[] = "iter_count";
         static constexpr char length_range[] = "length_range";
         static constexpr char map_size[] = "map_size";
         static constexpr char split_factor[] = "split_factor";
         static constexpr char multi_thread_test[] = "multi_thread_test";
         static constexpr char res_type[] = "res_type";
         static constexpr char single_thread_test[] = "single_thread_test";
         static constexpr char task_pool_range[] = "task_pool_range";
         static constexpr char thread_pool_range[] = "thread_pool_range";

         static constexpr char angle[] = "angle";
         static constexpr char scale[] = "scale";
         static constexpr char ordinate_bias[] = "ordinate_bias";
         static constexpr char abscissa_bias[] = "abscissa_bias";

         static constexpr char gcs_info[] = "gcs_info";
         static constexpr char mtx_info[] = "mtx_info";

         static constexpr char col_count[] = "col_count";
         static constexpr char row_count[] = "row_count";
      };
   };
}