#pragma once

#include "common/central_class.h"
#include "common/pathfinder_structs.h"
#include "../Multithread/MCManager.h"

namespace SV::pathfinder
{
   struct task_list_holder
   {
      std::vector<path_finder_task>& task_list;

      task_list_holder(std::vector<path_finder_task>& task_list)
         : task_list(task_list)
      {}
   };

   class ThreadSplitter
      : public Central
   {
   public:
      ThreadSplitter(central_pack* pack);
      ~ThreadSplitter();

      void SetSettings(const SV::pathfinder::path_finder_settings& stt);
      void CountCurrent(std::vector<path_finder_task>&, std::function<void(void)>);
      const research::task_holder_statistic::holder_cluster_run_history& GetStatisticHistory() const { return m_holderStatisticHistory; }
      void ClearStatisticHistory() { m_holderStatisticHistory.clear(); }
   protected:
      void onTaskPacketComputingFinished();
      void formatTaskPacket();
      task_unit pathFinderTaskToHolderTask(const path_finder_task&);
      bool nonComputedTasksExists();
   private:
      SV::pathfinder::path_finder_settings m_settings;
      std::shared_ptr<task_list_holder> m_taskList;
      std::unique_ptr<MultithreadComputingManager> m_taskManager;
      std::shared_ptr<TaskStorage> m_taskPacket;
      research::task_holder_statistic::holder_cluster_run_history m_holderStatisticHistory;
      std::function<void(void)> m_finalizeCallback;
   };
}