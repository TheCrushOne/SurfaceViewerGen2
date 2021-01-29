#include "stdafx.h"
#include "ThreadSplitter.h"

using namespace SV::pathfinder;

ThreadSplitter::ThreadSplitter(central_pack* pack)
   : m_taskManager(std::make_unique<MultithreadComputingManager>(pack))
   , m_taskPacket(std::make_shared<TaskStorage>())
   , Central(pack)
{
   // TODO: check!!!
   m_taskManager->SetTaskPacketFinishCallback([this]() { onTaskPacketComputingFinished(); });
}

ThreadSplitter::~ThreadSplitter()
{}

void ThreadSplitter::SetSettings(const SV::pathfinder::path_finder_settings& stt)
{
   m_settings = stt;
   // test 4/8
   //const int threadCount = 16;   // NOTE: По количеству логических ядер 8+HT
   m_taskManager->SetHolderCount(m_settings.multithread ? m_settings.thread_count : 1);
}

void ThreadSplitter::CountCurrent(std::vector<path_finder_task>& taskList, size_t unitCount, std::function<void(void)> callback)
{
   TaskHolder::ClearStatistic();
   m_holderStatisticHistory.unit_count = unitCount;
   m_taskList = std::make_shared<task_list_holder>(taskList);
   m_finalizeCallback = callback;
   m_currentPacketIdx = 0;
   onTaskPacketComputingFinished();
}

void ThreadSplitter::formatTaskPacket()
{
   m_taskPacket->clear();
   std::vector<path_finder_task>::iterator it = m_taskList->task_list.begin();
   while (it != m_taskList->task_list.end() && m_taskPacket->size() < m_settings.packet_size)
   //for (size_t idx = 0; idx < m_settings.packet_size && m_taskList.size() > 0; idx++)
   {
      if (!it->counted)
         m_taskPacket->emplace_back(pathFinderTaskToHolderTask(*it));
      it++;
   }
   m_currentPacketIdx++;
}

bool ThreadSplitter::nonComputedTasksExists()
{
   for (const auto& task : m_taskList->task_list)
   {
      if (!task.counted)
         return true;
   }
   return false;
}

void ThreadSplitter::onTaskPacketComputingFinished()
{
   static const unsigned long long int threadCountSpec = std::thread::hardware_concurrency();

   if (!nonComputedTasksExists())
   {
      m_taskManager->Finale();
      m_holderStatisticHistory.history.emplace_back(*m_taskManager->GetCurrentStatistic());
      //buildLandCoverage();
      m_finalizeCallback();
      return;
   }
   formatTaskPacket();

   m_taskManager->LaunchTaskPacket(m_taskPacket);
}

task_unit ThreadSplitter::pathFinderTaskToHolderTask(path_finder_task& path_task)
{
   task_unit holder_task;
   holder_task.status = TaskStatus::TS_QUEUED;
   holder_task.runnable = [&path_task]() {
      path_task.runnable(path_task.start, path_task.finish, path_task.logic, path_task.rawdata, path_task.coverageMatrix, &path_task.path_found);
      path_task.counted = true;
   };
   holder_task.unit_index = path_task.unit_index;
   holder_task.shard_index = path_task.shard_index;
   holder_task.packet_index = m_currentPacketIdx;

   return holder_task;
}