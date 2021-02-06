#pragma once

#include "common\central_class.h"
#include "TaskHolder.h"

namespace SV::pathfinder
{
   typedef std::pair<TaskHolder, TaskStatus> TaskHolderData;
   typedef std::vector<TaskHolderData> TaskHolders;
   typedef std::vector<task_unit> TaskStorage;

   class MultithreadComputingManager
      : public Central
   {
   public:
      MultithreadComputingManager(central_pack*);
      ~MultithreadComputingManager();

      void SetHolderCount(size_t count);
      void SetTaskPacketFinishCallback(TaskHolderGroupFinishCallback callback);

      void LaunchTaskPacket(std::shared_ptr<TaskStorage> taskPacket);
      void Finale();

      const research::task_holder_statistic::mcmanager_run_log* GetCurrentStatistic() { return TaskHolder::GetStatistic(); }
   private:
      TaskHolders m_holders;
      TaskHolderGroupFinishCallback m_callback;
   };
}