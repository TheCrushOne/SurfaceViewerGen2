#pragma once

#include "common/settings.h"
#include "common\communicator.h"
#include "common\pathfinder_structs.h"
#include "crossdllinterface\EngineInterface.h"
#include "algorithm\pathfinder\PathFinderPipeline.h"
//#include "base/instance.h"
#include "common/simulation_structs.h"
#include <vector>

//#define CURTIME_MS() QDateTime::currentDateTime().toMSecsSinceEpoch()
#define CURTIME_MS(time_ms) SYSTEMTIME time; \
                            GetSystemTime(&time); \
                            time_ms = (time.wSecond * 1000) + time.wMilliseconds;

namespace settings
{
   struct application_settings;
   struct research_settings;
   //struct gen1_Settings;
}

//template<class T>
//struct GlobalExperimentStatistic;

namespace engine
{
   struct TimeData
   {
      __int64 start;
      __int64 finish;
      __int64 diff;

      void diff() { diff = finish - start; }
   };

   struct TimeResearchComplexMeta
   {
      struct TimeResearchComplexIndex
      {

      } index;
      struct TimeResearchComplexResult
      {

      } result;
   };

   struct LengthResearchComplexMeta
   {
      struct LengthResearchComplexIndex
      {

      } index;
      struct LengthResearchComplexResult
      {

      } result;
   };

   struct ThreadResearchComplexMeta
   {
      struct ThreadResearchComplexIndex
      {
         size_t thread_pool_idx;
         size_t task_pool_idx;
         size_t fly_count_idx;

         size_t thread_pool_value;
         size_t task_pool_value;
         size_t fly_count_value;
      } index;

      struct ThreadResearchComplexResult
      {
         TimeData time;
      } result;
   };

   class Engine : public iEngine, public Communicable
   {
   public:
      Engine();
   public:
      void Init(ICommunicator* pCommunicator) { m_communicator = pCommunicator; m_pathfinder->Init(m_communicator); }
      void SetSettings(std::shared_ptr<settings::application_settings> settings) { m_appSettings = settings; }

      void LaunchResearch(const settings::research_settings& resStt);
      const std::vector<TimeResearchComplexMeta>& GetTimeResearchResult() { return m_timeTaskVct; }
      const std::vector<LengthResearchComplexMeta>& GetLengthResearchResult() { return m_lengthTaskVct; }
      const std::vector<ThreadResearchComplexMeta>& GetThreadResearchResult() { return m_threadTaskVct; }

      void ProcessPathFind(const ColregSimulation::scenario_data& scenarioData, const std::vector<std::vector<double>>& rawData, std::function<void(void)> completeCallback) override final;
      const pathfinder::route_data& GetLastProcessedPaths() const override final { return m_pathfinder->GetPaths(); }
      void Release() { delete this; }
   private:
      void timeResearch(const settings::research_settings& resStt/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen1>&*/);
      void lengthResearch(const settings::research_settings& resStt/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen2>&*/);
      void threadResearch(const settings::research_settings& resStt/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen3>&*/);

      void threadResNextStep();
      void generateResScenarioData(ColregSimulation::scenario_data& data, const ThreadResearchComplexMeta::ThreadResearchComplexIndex& idx);

      void processPathFind(const ColregSimulation::scenario_data& scenarioData, const std::vector<std::vector<double>>& rawData, std::function<void(void)> completeCallback);
      void generateResMap(size_t mapSize/*std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, const STT::Gen1Settings&*/);
      pathfinder::check_fly_zone_result checkFlyZone(float);
      void convertMap(const std::vector<std::vector<double>>& rawdataSrc, std::shared_ptr<pathfinder::Matrix<SVCG::route_point>> rawdataDst);
      //CheckGoZoneResult checkGoZone(const std::shared_ptr<QHeightMapSurfaceDataProxy>, int, int, int, int);
      //CheckGoZoneResult checkAngles(const QSurfaceDataItem*, const QSurfaceDataItem*, const QSurfaceDataItem*, const QSurfaceDataItem*, const QSurfaceDataItem*);
      //SurfaceElement recountElement(const std::shared_ptr<QHeightMapSurfaceDataProxy>, int, int, int, int);
   //signals:
   protected:
      /*void converted() {  }
      void percent(int prcnt) {  }
      void drop() {  }*/
   private:
      std::shared_ptr<pathfinder::path_finder_indata> m_indata;
      std::shared_ptr<settings::application_settings> m_appSettings;
      std::unique_ptr<pathfinder::PathFinderPipeline> m_pathfinder;

      std::shared_ptr<pathfinder::Matrix<SVCG::route_point>> m_rawdata;
      std::shared_ptr<pathfinder::route_data> m_routedata;
      //std::shared_ptr<pathfinder::route_data> m_routedata;

      // TODO: восстановить
      std::vector<TimeResearchComplexMeta> m_timeTaskVct;
      std::vector<LengthResearchComplexMeta> m_lengthTaskVct;

      std::vector<ThreadResearchComplexMeta> m_threadTaskVct;
      size_t m_threadTaskCurrentIdx;
   };
}