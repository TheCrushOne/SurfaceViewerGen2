#pragma once
#include <functional>
#include "common/pathfinder_structs.h"

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

   class TaskHolder
   {
   public:
      void Launch(std::shared_ptr<std::vector<task_unit>> taskPacket, std::function<void(void)> callback)
      {
         this->callback = callback;
         packet = taskPacket;
         onFinished();  // NOTE: принудительный старт
      }
   protected:
      void onFinished()
      {
         status = HolderStatus::HS_FINISHED;
         for (auto& task : *packet.get())
         {
            if (task.status == TaskStatus::TS_QUEUED)
            {
               task.status = TaskStatus::TS_ONCOUNT;
               status = HolderStatus::HS_RUNNING;
               std::thread(&launchSingleTask, this, task).detach();
               break;
            }
         }
         if (status == HolderStatus::HS_FINISHED)
            callback();
      }
      void launchSingleTask(task_unit& task)
      {
         task.runnable();
         onFinished();
      }
   private:
      HolderStatus status;
      std::shared_ptr<std::vector<task_unit>> packet;
      std::function<void(void)> callback;
   };
}