#pragma once

#include <vector>

namespace SV::research
{
   struct task_holder_statistic
   {
      struct statistic_unit
      {
         size_t holder_idx;
         size_t task_idx;
         __int64 start_ts;
         __int64 finish_ts;
      };

      std::vector<statistic_unit> stat_data;
   };

   typedef std::vector<research::task_holder_statistic::statistic_unit> statistic_data;
   typedef std::vector<statistic_data> statistic_data_history;
}