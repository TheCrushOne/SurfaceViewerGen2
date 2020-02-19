#pragma once

#include "common/settings.h"
#include "dispatchableinterface.h"
//#include "base/instance.h"
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

class Engine : public iDispatchable
{
public:
   Engine(const std::shared_ptr<settings::application_settings>);
public:
   void OnScenarioLoad() override;
   void SetSettings(std::shared_ptr<settings::application_settings> settings) { m_appSettings = settings; }
   //void ConvertMap(const std::shared_ptr<QHeightMapSurfaceDataProxy>, std::shared_ptr<SVM::iMatrix<SurfaceElement>> &, std::shared_ptr<STT::ApplicationSettings> &);
   void TimeResearchGen1(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen1>&*/);
   void LengthResearchGen2(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen2>&*/);
   void ThreadResearchGen3(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen3>&*/);
private:
   void generateResMap(/*std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, const STT::Gen1Settings&*/);
   surface_viewer::check_fly_zone_result checkFlyZone(float);
   //CheckGoZoneResult checkGoZone(const std::shared_ptr<QHeightMapSurfaceDataProxy>, int, int, int, int);
   //CheckGoZoneResult checkAngles(const QSurfaceDataItem*, const QSurfaceDataItem*, const QSurfaceDataItem*, const QSurfaceDataItem*, const QSurfaceDataItem*);
   //SurfaceElement recountElement(const std::shared_ptr<QHeightMapSurfaceDataProxy>, int, int, int, int);
//signals:
   void converted() {  }
   void percent(int prcnt) {  }
   void drop() {  }
private:
   std::shared_ptr<settings::application_settings> m_appSettings;
};
