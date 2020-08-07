#pragma once

#include "common\central_class.h"
#include "TaskHolder.h"

namespace pathfinder
{
   using TaskHolderData = std::pair<TaskHolder, TaskStatus>;
   using TaskHolders = std::vector<TaskHolderData>;
   using TaskStorage = std::vector<task_unit>;

   class MultithreadComputingManager : public Central
   {
   public:
      MultithreadComputingManager(central_pack*);
      ~MultithreadComputingManager();

      void SetHolderCount(size_t count);
      void SetTaskPacketFinishCallback(std::function<void(void)> callback);

      void LaunchTaskPacket(std::shared_ptr<TaskStorage> taskPacket);
      void Finale();
   private:
      TaskHolders m_holders;
      std::function<void(void)> m_callback;
   };
}