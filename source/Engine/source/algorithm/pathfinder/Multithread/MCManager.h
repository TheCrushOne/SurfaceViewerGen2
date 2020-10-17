#pragma once

#include "common\central_class.h"
#include "TaskHolder.h"

namespace SV::pathfinder
{
   typedef std::pair<TaskHolder, TaskStatus> TaskHolderData;
   typedef std::vector<TaskHolderData> TaskHolders;
   typedef std::vector<task_unit> TaskStorage;

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