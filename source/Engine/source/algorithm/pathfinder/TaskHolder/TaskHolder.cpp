#include "stdafx.h"
#include "TaskHolder.h"

using namespace pathfinder;
std::recursive_mutex g_mutex;

void TaskHolder::Launch(std::shared_ptr<std::vector<task_unit>> taskPacket, std::function<void(void)> callback)
{
   this->callback = callback;
   packet = taskPacket;
   onFinished(true);  // NOTE: принудительный старт
}

void TaskHolder::onFinished(bool fromLaunch)
{
   std::lock_guard<std::recursive_mutex> guard(g_mutex);
   status = HolderStatus::HS_FINISHED;
   GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "task finished: thread [%d], from launch [%i], packet size [%d]", std::this_thread::get_id(), fromLaunch, packet->size());
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
   using namespace std::chrono_literals;
   std::this_thread::sleep_for(2s);
   //task.runnable();
   task.status = TaskStatus::TS_FINISHED;
   onFinished(false);
}