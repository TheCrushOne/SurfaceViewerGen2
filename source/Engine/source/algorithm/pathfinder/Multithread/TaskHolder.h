#pragma once
#include <functional>
#include <thread>
#include "Semaphore.h"
#include "common/pathfinder_structs.h"
#include "common/central_class.h"

namespace pathfinder
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
      size_t index;
      //std::function<settings::route(settings::route&, const std::shared_ptr<Matrix<SVCG::route_point>>, size_t, bool)> runnable;
      TaskStatus status;
   };

   class TaskHolder : public Central
   {
   public:
      TaskHolder(central_pack*);
      ~TaskHolder();

      static void SetTaskPacket(std::shared_ptr<std::vector<task_unit>> taskPacket) { m_packet = taskPacket; }

      void Launch();

      static void InitSynchronizer();
      static void DeInitSynchronizer();
      static void SetTaskPacketFinishCallback(std::function<void(void)> callback) { m_callback = callback; }

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

      static std::shared_ptr<std::vector<task_unit>> m_packet;
      static std::unique_ptr<SemaphoreType> m_sema;
      static std::function<void(void)> m_callback;
      static bool m_crsRaised;
   };
}