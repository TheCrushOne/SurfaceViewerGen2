#pragma once

#include "colreg/ModuleGuard.h"
#include "common/header_collector.h"
#include "common\communicator.h"
#include "common\pathfinder_structs.h"
#include "crossdllinterface\EngineInterface.h"
#include "algorithm\pathfinder\PathFinderPipeline.h"
//#include "base/instance.h"
#include "common/simulation_structs.h"
#include "common/research_structs.h"
#include "common/central_class.h"
#include "settings/settings.h"
#include <vector>

//#define CURTIME_MS() QDateTime::currentDateTime().toMSecsSinceEpoch()

#define CURTIME_MS(time_ms) time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

//template<class T>
//struct GlobalExperimentStatistic;

namespace engine
{
   class Engine : public iEngine, public Central
   {
   public:
      Engine(central_pack* pack);
   public:
      void LaunchResearch(std::function<void(void)>) override final;
      const TimeResearchComplexStorage& GetTimeResearchResult() const override final { return m_timeResStorage; }
      const LengthResearchComplexStorage& GetLengthResearchResult() const override final { return m_lengthResStorage; }
      const ThreadResearchComplexStorage& GetThreadResearchResult() const override final { return m_threadResStorage; }

      void ProcessPathFind(const ColregSimulation::scenario_data&, const pathfinder::GeoMatrix&, std::shared_ptr<settings::application_settings>, std::function<void(void)>) override final;
      const pathfinder::route_data& GetLastProcessedPaths() const override final { return m_pathfinder->GetPaths(); }
      void Release() override final { delete this; }
   private:
      void timeResearch();
      void lengthResearch();
      void threadResearch();

      void logThreadResearchResult();

      void threadResNextStep();
      void generateResScenarioData(ColregSimulation::scenario_data&, const settings::research_settings&, const ThreadResearchComplexStorage::SuperCell::Index&);

      void processPathFind(const ColregSimulation::scenario_data&, const pathfinder::GeoMatrix&, std::function<void(void)>);
      void processPathFindInternal(const ColregSimulation::scenario_data&, pathfinder::path_finder_settings, std::function<void(void)>);
      void generateResMap(size_t mapSize);
      pathfinder::check_fly_zone_result checkFlyZone(const pathfinder::GeoMatrix&, int, int);
      void convertMap(const pathfinder::GeoMatrix&, std::shared_ptr<pathfinder::RoutePointMatrix>);
      pathfinder::check_go_zone_result checkGoZone(const pathfinder::GeoMatrix&, int, int);
      pathfinder::check_go_zone_result checkAngles(double, double, double, double, double, double, double, double, double);
      //SurfaceElement recountElement(const std::shared_ptr<QHeightMapSurfaceDataProxy>, int, int, int, int);
   //signals:
   protected:
      /*void converted() {  }
      void percent(int prcnt) {  }
      void drop() {  }*/
   private:
      std::shared_ptr<pathfinder::path_finder_indata> m_indata;
      std::unique_ptr<pathfinder::PathFinderPipeline> m_pathfinder;
      std::shared_ptr<settings::application_settings> m_settings;

      std::shared_ptr<pathfinder::RoutePointMatrix> m_rawdata;
      std::shared_ptr<pathfinder::route_data> m_routedata;
      //std::shared_ptr<pathfinder::route_data> m_routedata;
      std::function<void(void)> m_endRoundCallback;

      // TODO: ������������
      TimeResearchComplexStorage m_timeResStorage;
      LengthResearchComplexStorage m_lengthResStorage;

      ThreadResearchComplexStorage m_threadResStorage;
      size_t m_threadTaskCurrentIdx;
   };
}