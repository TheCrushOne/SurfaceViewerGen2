#include "stdafx.h"
#include "TaskHolder.h"

using namespace pathfinder;
std::recursive_mutex g_mutex;

void TaskHolder::Launch(std::shared_ptr<std::vector<task_unit>> taskPacket, std::function<void(void)> callback)
{
   this->callback = callback;
   packet = taskPacket;
   onFinished();  // NOTE: принудительный старт
}

void TaskHolder::onFinished()
{
   std::lock_guard<std::recursive_mutex> guard(g_mutex);
   status = HolderStatus::HS_FINISHED;
   for (task_unit& task : *packet.get())
   {
      if (task.status == TaskStatus::TS_QUEUED)
      {
         task.status = TaskStatus::TS_ONCOUNT;
         status = HolderStatus::HS_RUNNING;
         std::thread(&TaskHolder::launchSingleTask, this, std::ref(task)).detach();
         break;
      }
   }
   if (status == HolderStatus::HS_FINISHED)
      callback();
}

void TaskHolder::launchSingleTask(task_unit& task)
{
   task.runnable();
   task.status = TaskStatus::TS_FINISHED;
   onFinished();
}