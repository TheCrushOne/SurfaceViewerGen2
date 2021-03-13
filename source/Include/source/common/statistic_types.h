#pragma once

#include <vector>

namespace SV::research::task_holder_statistic
{
   //struct statistic_unit
   //{
   //   //size_t holder_idx;
   //   size_t task_idx;
   //   size_t unit_idx;
   //   size_t shard_idx;
   //   size_t packet_idx;
   //   __int64 start_ts;
   //   __int64 finish_ts;
   //};

   //// NOTE: статистика по одному холдеру за один запуск расчета(одна итерация эксперимента)
   //struct holder_run_data
   //{
   //   std::vector<statistic_unit> data;
   //};
   //// NOTE: данные по всем холдерам за один запуск
   //struct holder_cluster_run_data
   //{
   //   std::unordered_map<size_t, holder_run_data> data;
   //};
   //// NOTE: данные за все запуски расчета в пределах одного поиска назменого пути
   //struct holder_cluster_run_history
   //{
   //   size_t unit_count;
   //   std::vector<holder_cluster_run_data> history;
   //};
   //// NOTE: данные по всем экспериментам
   //struct experiment_history
   //{
   //   std::vector<holder_cluster_run_history> history;
   //};

   // NOTE: данные по одной таске
   struct mcmanager_task_log
   {
      size_t task_idx;
      size_t unit_idx;
      size_t shard_idx;
      size_t holder_idx;

      __int64 start_ts;
      __int64 finish_ts;
   };

   // NOTE: данные по запуску одного пакета
   struct mcmanager_packet_log
   {
      size_t packet_idx;
      __int64 start_ts;
      __int64 finish_ts;
      std::vector<mcmanager_task_log> task_list;

      // NOTE: experimetnal
      //std::vector<bool> readiness_list;
   };

   // NOTE: данные по запуску одной групп задач(одной итерации пайплайна)
   struct mcmanager_run_log
   {
      size_t unit_count;
      std::vector<mcmanager_packet_log> packet_list;
   };

   // NOTE: данные по запуску одного полного расчета пути(пайплайна)
   struct mcmanager_pipeline_run_log
   {
      size_t thread_count;
      size_t packet_size;
      size_t split_factor;
      std::vector<mcmanager_run_log> step_list;
   };

   // NOTE: данные по всем запускам пайплайна в пределах времени жизни приложения
   struct mcmanager_overall_log
   {
      std::vector<mcmanager_pipeline_run_log> pipeline_run_list;
   };
}