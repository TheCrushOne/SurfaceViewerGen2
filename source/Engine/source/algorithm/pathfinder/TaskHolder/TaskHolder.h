#pragma once
#include <functional>
#include <thread>
#include "common/pathfinder_structs.h"
#include "common/central_class.h"

namespace pathfinder
{
   enum class TaskStatus : unsigned short
   {
      TS_QUEUED = 0,
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
      //std::function<settings::route(settings::route&, const std::shared_ptr<Matrix<SVCG::route_point>>, size_t, bool)> runnable;
      TaskStatus status;
   };

   class TaskHolder : public Central
   {
   public:
      void Launch(std::shared_ptr<std::vector<task_unit>> taskPacket, std::function<void(void)> callback);
   protected:
      void onFinished(bool);
   public:
      void launchSingleTask(task_unit& task);
   private:
      HolderStatus status;
      std::shared_ptr<std::vector<task_unit>> packet;
      std::function<void(void)> callback;
   };
}