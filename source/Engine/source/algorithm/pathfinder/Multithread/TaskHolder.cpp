#include "stdafx.h"
#include "TaskHolder.h"
#include <time.h>

#define CURTIME_MS_EPOCH() clock();//std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

using namespace SV;
using namespace SV::pathfinder;
std::recursive_mutex g_mutex;

std::shared_ptr<research::task_holder_statistic> TaskHolder::m_stat = std::make_shared<research::task_holder_statistic>();
std::shared_ptr<std::vector<task_unit>> TaskHolder::m_packet = nullptr;
std::unique_ptr<SemaphoreType> TaskHolder::m_sema = nullptr;
TaskHolderGroupFinishCallback TaskHolder::m_callback = nullptr;
__int64 TaskHolder::m_startTime = 0;
bool TaskHolder::m_crsRaised = false;

CRITICAL_SECTION critical_inner, critical_outer, critical_chck;

TaskHolder::TaskHolder(central_pack* pack)
   : Central(pack)
   , status(HolderStatus::HS_IDLE)
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

void TaskHolder::FixCurrentTime()
{
   TaskHolder::m_startTime = CURTIME_MS_EPOCH();
}

void TaskHolder::ClearStatistic()
{
   TaskHolder::m_stat.get()->stat_data.clear();
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
   m_callback(TaskHolder::m_stat.get());
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
   task.start_ts = CURTIME_MS_EPOCH() - m_startTime;
   task.runnable();
   task.finish_ts = CURTIME_MS_EPOCH() - m_startTime;
   task.holder_idx = holder_idx;
   EnterCriticalSection(&critical_inner);
   m_stat.get()->stat_data.emplace_back(research::task_holder_statistic::statistic_unit{ task.holder_idx, task.index, task.start_ts, task.finish_ts });
   //GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "task finished: thread [%d], packet size [%d], idx [%d]", std::this_thread::get_id(), m_packet->size(), task.index);
   task.status = TaskStatus::TS_FINISHED;
   status = HolderStatus::HS_FINISHED;
   onFinished(false);
}