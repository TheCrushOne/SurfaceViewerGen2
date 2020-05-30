#include "stdafx.h"
#include "TaskHolder.h"

using namespace pathfinder;
std::recursive_mutex g_mutex;

std::unique_ptr<SemaphoreType> TaskHolder::m_sema = nullptr;
std::shared_ptr<std::vector<task_unit>> TaskHolder::m_packet = nullptr;
std::function<void(void)> TaskHolder::m_callback = nullptr;
bool TaskHolder::m_crsRaised = false;

CRITICAL_SECTION critical_inner, critical_outer, critical_chck;

TaskHolder::TaskHolder()
   : status(HolderStatus::HS_IDLE)
{}

TaskHolder::~TaskHolder()
{}

void TaskHolder::InitSynchronizer()
{
   InitializeCriticalSection(&critical_inner);
   InitializeCriticalSection(&critical_outer);
   InitializeCriticalSection(&critical_chck);
   m_crsRaised = true;
}

void TaskHolder::DeInitSynchronizer()
{
   if (m_crsRaised)
   {
      m_crsRaised = false;
      DeleteCriticalSection(&critical_inner);
      DeleteCriticalSection(&critical_outer);
      DeleteCriticalSection(&critical_chck);
   }
}

void TaskHolder::ForceInnerLock()
{
   EnterCriticalSection(&critical_inner);
}

void TaskHolder::ForceInnerUnlock()
{
   LeaveCriticalSection(&critical_inner);
}

void TaskHolder::Launch()
{
   EnterCriticalSection(&critical_outer);
   onFinished(true);  // NOTE: принудительный старт
}

void TaskHolder::finish()
{
   m_callback();
}

void TaskHolder::onTaskHolderFinished()
{
   EnterCriticalSection(&critical_inner);
   //std::string status;
   for (const auto& task : *m_packet.get())
   {
      if (task.status != TaskStatus::TS_FINISHED)
      {
         //status += "0";
         //GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "task status: [%s]", status.c_str());
         LeaveCriticalSection(&critical_inner);
         return;
      }
      //status += "1";
   }
   // WARNING: тут может падать!!!
   //GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "task status: [%s]", status.c_str());
   LeaveCriticalSection(&critical_inner);
   finish();
}

void TaskHolder::onFinished(bool fromLaunch)
{
   //std::lock_guard<std::recursive_mutex> guard(g_mutex);
   //TaskHolder::m_sema->lock();
   //GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "task finished: from launch [%i]", fromLaunch);
   for (auto& task : *m_packet.get())
   {
      if (task.status == TaskStatus::TS_QUEUED)
      {
         task.status = TaskStatus::TS_ONCOUNT;
         status = HolderStatus::HS_RUNNING;
         std::thread(&TaskHolder::launchSingleTask, this, std::ref(task)).detach();
         LeaveCriticalSection(&(fromLaunch ? critical_outer : critical_inner));
         return;
      }
   }
   LeaveCriticalSection(&(fromLaunch ? critical_outer : critical_inner));
   if (status == HolderStatus::HS_FINISHED)
      onTaskHolderFinished();
   //TaskHolder::m_sema->unlock();
}

void TaskHolder::launchSingleTask(task_unit& task)
{
   task.runnable();
   EnterCriticalSection(&critical_inner);
   //GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "task finished: thread [%d], packet size [%d], idx [%d]", std::this_thread::get_id(), m_packet->size(), task.index);
   task.status = TaskStatus::TS_FINISHED;
   status = HolderStatus::HS_FINISHED;
   onFinished(false);
}