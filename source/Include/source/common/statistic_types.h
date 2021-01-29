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
      size_t packet_idx;
      __int64 start_ts;
      __int64 finish_ts;
   };

   // NOTE: ���������� �� ������ ������� �� ���� ������ �������(���� �������� ������������)
   struct holder_run_data
   {
      std::vector<statistic_unit> data;
   };
   // NOTE: ������ �� ���� �������� �� ���� ������
   struct holder_cluster_run_data
   {
      std::unordered_map<size_t, holder_run_data> data;
   };
   // NOTE: ������ �� ��� ������� ������� � �������� ������ ������ ��������� ����
   struct holder_cluster_run_history
   {
      size_t unit_count;
      std::vector<holder_cluster_run_data> history;
   };
   // NOTE: ������ �� ���� �������������
   struct experiment_history
   {
      std::vector<holder_cluster_run_history> history;
   };
}