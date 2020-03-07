#pragma once

#include "common/settings.h"
#include "common\communicator.h"
#include "common\pathfinder_structs.h"
#include "crossdllinterface\EngineInterface.h"
#include "algorithm\pathfinder\pathfinder.h"
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

struct research_result
{
   //std::shared_ptr<SVM::iHyperCell<float>> dataStorage;
};

struct research_result_gen1 : public research_result
{
   /*BaseGeometryElement singleRhomboid, multiRhomboid;
   BaseGeometryElement singleSnake, multiSnake;
   BaseGeometryElement singleSector, multiSector;

   ResearchResultElement(BaseGeometryElement sr, BaseGeometryElement mr, BaseGeometryElement ssn, BaseGeometryElement msn, BaseGeometryElement sse, BaseGeometryElement mse)
      : singleRhomboid(sr), multiRhomboid(mr)
      , singleSnake(ssn), multiSnake(msn)
      , singleSector(sse), multiSector(mse)
   {}
   ResearchResultElement()
   {}*/
};

struct research_result_gen2 : public research_result
{};

struct research_result_gen3 : public research_result
{};

namespace engine
{
   class Engine : public iEngine, public Communicable
   {
   public:
      Engine();
   public:
      void SetSettings(std::shared_ptr<settings::application_settings> settings) { m_appSettings = settings; }
      
      void TimeResearchGen1(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen1>&*/);
      void LengthResearchGen2(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen2>&*/);
      void ThreadResearchGen3(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen3>&*/);
      //void SetCommunicator(ICommunicator* comm);
      void ProcessPathFind(ColregSimulation::scenario_data& scenarioData, const std::vector<std::vector<double>>& rawData) override final;
      void Release() { delete this; }
   private:
      void generateResMap(/*std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, const STT::Gen1Settings&*/);
      pathfinder::check_fly_zone_result checkFlyZone(float);
      void convertMap(const std::vector<std::vector<double>>& rawdataSrc, std::shared_ptr<pathfinder::Matrix<SVCG::route_point>> rawdataDst);
      void convertData(const settings::unit_source_data& rawdataSrc, std::shared_ptr<pathfinder::route_data> rawdataDst);
      void pathDump(const std::shared_ptr<pathfinder::route_data> rawdataSrc, settings::unit_source_data& rawdataDst);
      //CheckGoZoneResult checkGoZone(const std::shared_ptr<QHeightMapSurfaceDataProxy>, int, int, int, int);
      //CheckGoZoneResult checkAngles(const QSurfaceDataItem*, const QSurfaceDataItem*, const QSurfaceDataItem*, const QSurfaceDataItem*, const QSurfaceDataItem*);
      //SurfaceElement recountElement(const std::shared_ptr<QHeightMapSurfaceDataProxy>, int, int, int, int);
   //signals:
   protected:
      /*void converted() {  }
      void percent(int prcnt) {  }
      void drop() {  }*/
   private:
      std::shared_ptr<settings::application_settings> m_appSettings;
      std::unique_ptr<pathfinder::PathFinder> m_pathfinder;

      std::shared_ptr<pathfinder::Matrix<SVCG::route_point>> m_rawdata;
      std::shared_ptr<pathfinder::route_data> m_routedata;
      //std::shared_ptr<pathfinder::route_data> m_routedata;
   };
}