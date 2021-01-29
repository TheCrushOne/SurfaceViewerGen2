#pragma once
#include <functional>
#include <thread>
#include "Semaphore.h"
#include "common/pathfinder_structs.h"
#include "common/central_class.h"
#include "common/statistic_types.h"

namespace SV::pathfinder
{
   enum class TaskStatus : unsigned short
   {
      TS_IDLE = 0,
      TS_QUEUED,
      TS_ONCOUNT,
      TS_FINISHED,
   };

   enum class HolderStatus : unsigned short
   {
      HS_IDLE = 0,   // служебный
      HS_RUNNING,
      HS_FINISHED,
   };

   struct task_unit
   {
      std::function<void(void)> runnable;
      size_t task_index;
      size_t unit_index;
      size_t shard_index;
      size_t packet_index;
      unsigned long start_ts, finish_ts;
      size_t holder_idx;
      //std::function<settings::route(settings::route&, const std::shared_ptr<Matrix<SVCG::route_point>>, size_t, bool)> runnable;
      TaskStatus status;
   };

   typedef std::function<void(void)> TaskHolderGroupFinishCallback;

   class TaskHolder
      : public Central
   {
   public:
      TaskHolder(central_pack*);
      ~TaskHolder();

      static void SetTaskPacket(std::shared_ptr<std::vector<task_unit>> taskPacket) { m_packet = taskPacket; }

      void Launch();
      void SetIdx(size_t idx) { holder_idx = idx; }

      static void InitSynchronizer();
      static void DeInitSynchronizer();
      static void ClearStatistic();
      static const research::task_holder_statistic::holder_cluster_run_data* GetStatistic() { return m_stat.get(); }
      static void SetTaskPacketFinishCallback(TaskHolderGroupFinishCallback callback) { m_callback = callback; }

      static void FixCurrentTime();
      // NOTE: они не совсем жесткие, т.е. это скорее try
      static void ForceInnerLock();
      static void ForceInnerUnlock();
   protected:
      void finish();
      void onFinished(bool);
      void onTaskHolderFinished();
   public:
      void launchSingleTask(task_unit& task);
   private:
      HolderStatus status;
      std::function<void(void)> callback;
      size_t holder_idx;

      static std::shared_ptr<research::task_holder_statistic::holder_cluster_run_data> m_stat;
      static std::shared_ptr<std::vector<task_unit>> m_packet;
      static std::unique_ptr<SemaphoreType> m_sema;
      static TaskHolderGroupFinishCallback m_callback;
      static bool m_crsRaised;
      static __int64 m_startTime;
   };
}