#pragma once

#include "colreg/ModuleGuard.h"
#include "common/settings.h"
#include "common\communicator.h"
#include "common\pathfinder_structs.h"
#include "crossdllinterface\EngineInterface.h"
#include "algorithm\pathfinder\PathFinderPipeline.h"
//#include "base/instance.h"
#include "common/simulation_structs.h"
#include "common/research_structs.h"
#include "common/central_class.h"
#include <vector>

//#define CURTIME_MS() QDateTime::currentDateTime().toMSecsSinceEpoch()

#define CURTIME_MS(time_ms) time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

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
   class Engine : public iEngine, public Central
   {
   public:
      Engine();
   public:
      void Init(central_pack* pack) { Central::Init(pack); m_pathfinder->Init(pack); }

      void LaunchResearch(std::function<void(void)>) override final;
      const TimeResearchComplexStorage& GetTimeResearchResult() const override final { return m_timeResStorage; }
      const LengthResearchComplexStorage& GetLengthResearchResult() const override final { return m_lengthResStorage; }
      const ThreadResearchComplexStorage& GetThreadResearchResult() const override final { return m_threadResStorage; }

      void ProcessPathFind(const ColregSimulation::scenario_data& scenarioData, const std::vector<std::vector<double>>& rawData, std::function<void(void)> completeCallback) override final;
      const pathfinder::route_data& GetLastProcessedPaths() const override final { return m_pathfinder->GetPaths(); }
      void Release() { delete this; }
   private:
      void timeResearch(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen1>&*/);
      void lengthResearch(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen2>&*/);
      void threadResearch(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen3>&*/);

      void logThreadResearchResult();

      void threadResNextStep();
      void generateResScenarioData(ColregSimulation::scenario_data& data, const settings::research_settings&, const ThreadResearchComplexStorage::SuperCell::Index& idx);

      void processPathFind(const ColregSimulation::scenario_data& scenarioData, const std::vector<std::vector<double>>& rawData, std::function<void(void)> completeCallback);
      void processPathFindInternal(const ColregSimulation::scenario_data& scenarioData, pathfinder::path_finder_settings stt, std::function<void(void)> completeCallback);
      void generateResMap(size_t mapSize/*std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, const STT::Gen1Settings&*/);
      pathfinder::check_fly_zone_result checkFlyZone(const std::vector<std::vector<double>>&, int, int);
      void convertMap(const std::vector<std::vector<double>>& rawdataSrc, std::shared_ptr<pathfinder::Matrix<SVCG::route_point>> rawdataDst);
      pathfinder::check_go_zone_result checkGoZone(const std::vector<std::vector<double>>&, int, int);
      pathfinder::check_go_zone_result checkAngles(double center, double left, double right, double top, double bottom, double topleft, double bottomleft, double topright, double bottomright);
      //SurfaceElement recountElement(const std::shared_ptr<QHeightMapSurfaceDataProxy>, int, int, int, int);
   //signals:
   protected:
      /*void converted() {  }
      void percent(int prcnt) {  }
      void drop() {  }*/
   private:
      std::shared_ptr<pathfinder::path_finder_indata> m_indata;
      std::unique_ptr<pathfinder::PathFinderPipeline> m_pathfinder;

      std::shared_ptr<pathfinder::Matrix<SVCG::route_point>> m_rawdata;
      std::shared_ptr<pathfinder::route_data> m_routedata;
      //std::shared_ptr<pathfinder::route_data> m_routedata;
      std::function<void(void)> m_endRoundCallback;

      // TODO: восстановить
      TimeResearchComplexStorage m_timeResStorage;
      LengthResearchComplexStorage m_lengthResStorage;

      ThreadResearchComplexStorage m_threadResStorage;
      size_t m_threadTaskCurrentIdx;
   };
}