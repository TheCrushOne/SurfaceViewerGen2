#pragma once

#include <vector>

namespace SV::research::task_holder_statistic
{
   struct statistic_unit
   {
      //size_t holder_idx;
      size_t task_idx;
      size_t unit_idx;
      size_t shard_idx;
      __int64 start_ts;
      __int64 finish_ts;
   };

   // NOTE: статистика по одному холдеру за один запуск расчета(одна итерация эксперимента)
   typedef std::vector<statistic_unit> holder_run_data;
   // NOTE: данные по всем холдерам за один запуск
   typedef std::unordered_map<size_t, holder_run_data> holder_cluster_run_data;
   // NOTE: данные за все запуски расчета в пределах одного поиска назменого пути
   typedef std::vector<holder_cluster_run_data> holder_cluster_run_history;
   // NOTE: данные по всем экспериментам
   typedef std::vector<holder_cluster_run_history> experiment_history;
}