#pragma once

#include "common\central_class.h"
#include "common/pathfinder_structs.h"
#include "common/statistic_types.h"

namespace SV::pathfinder
{
   class PathfinderStatistic
   {
   public:
      PathfinderStatistic() {};
      ~PathfinderStatistic() {};
   public:
      static void Clear();

      static void AddPipelineStat(size_t thread_count, size_t task_pool_size, size_t split_factor);
      static void AddStepStat(size_t unitCount);
      static void AddPacketStat();
      static void AddTaskStat();

      static void CommitStatStamp(research::task_holder_statistic::mcmanager_task_log task);
      static void CommitPacketStampStart(long long start);
      static void CommitPacketStampFinish(long long finish);

      static const research::task_holder_statistic::mcmanager_overall_log& GetStatistic() { return *m_stat.get(); }
   private:
      static std::shared_ptr<research::task_holder_statistic::mcmanager_overall_log> m_stat;
   };
}