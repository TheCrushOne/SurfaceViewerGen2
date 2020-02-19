#pragma once

//#include "base/instance.h"
#include "main/engine.h"
#include <memory>
#include "algorithm/statistic.h"
#include <functional>
#include <iostream>
#include <algorithm>
#include "common/coordinates.h"
#include "SVCG/route_point.h"

namespace SVM
{
   template<class T>
   class iMatrix;
}

// Xtremely Fast Matrix
namespace XFM
{
   template<typename type>
   struct Wrapper
   {
      Wrapper(type val)
         : data(nullptr)
         , rowCount(0)
         , colCount(0)
         , inval(val)
      {}
      type **data;
      size_t rowCount;
      size_t colCount;
      type inval;

      type* operator[](size_t idx) { return data[idx]; }
   };

   template<typename type>
   void CreateMatrix(Wrapper<type>& wrapper, size_t nrCount, size_t ncCount)
   {
      if (wrapper.colCount != ncCount)
      {
         for (size_t idx = 0; idx < wrapper.rowCount; idx++)
         {
            type *newbuf = new type[ncCount];
            //std::copy_n(wrapper.data, std::min(wrapper.colCount, ncCount), newbuf);
            delete[] wrapper.data[idx];
            wrapper.data[idx] = newbuf;
         }
         wrapper.colCount = ncCount;
      }
      if (wrapper.rowCount != nrCount)
      {
         type **newbuf = new type*[nrCount];
         for (size_t idx = 0; idx < std::min(wrapper.rowCount, nrCount); idx++)
         {
            newbuf[idx] = wrapper.data[idx];
         }
         wrapper.data = newbuf;

         if (wrapper.rowCount > nrCount)
         {
            for (size_t ridx = nrCount; ridx < wrapper.rowCount; ridx++)
               delete [] wrapper.data[ridx];
         }
         else
         {
            for (size_t ridx = wrapper.rowCount; ridx < nrCount; ridx++)
               wrapper.data[ridx] = new type[wrapper.colCount];
         }
      }
      for (size_t ridx = 0; ridx < wrapper.rowCount; ridx++)
      {
         for (size_t cidx = 0; cidx < wrapper.colCount; cidx++)
         {
            wrapper.data[ridx][cidx] = wrapper.inval;
         }
      }
   }

   template<typename type>
   void DeleteMatrix(Wrapper<type>& wrapper)
   {
      for (size_t idx = 0; idx < wrapper.rowCount; idx++)
         delete [] wrapper.data[idx];
      delete [] wrapper.data;
      wrapper.data = nullptr;
      wrapper.rowCount = 0;
      wrapper.colCount = 0;
   }
}


enum StrategyType : unsigned short
{
   ST_RHOMBOID = 0,
   ST_SECTOR,
   ST_SNAKE
};

struct StrategySettings
{
   StrategyType type;
   double radius;
};



//struct MatrixPoint
//{
//   size_t row, col;
//   MatrixPoint(size_t row, size_t col)
//      : row(row)
//      , col(col)
//   {}
//   MatrixPoint()
//      : row(0)
//      , col(0)
//   {}
//};

/*struct CombinedPoint
{
   RoutePoint rp;
   MatrixPoint mp;
};*/

struct Route
{
   SVCG::route_point start;
   SVCG::route_point finish;
   std::vector<SVCG::route_point> controlPointList;
   std::vector<SVCG::route_point> route;
   Route() {}
   Route(SVCG::route_point& start, SVCG::route_point& finish)
      : start(start)
      , finish(finish)
      , controlPointList()
      , route()
   {}
   Route(SVCG::route_point& start, SVCG::route_point& finish, std::vector<SVCG::route_point>& route, std::vector<SVCG::route_point>& controlPointList)
      : start(start)
      , finish(finish)
      , controlPointList(controlPointList)
      , route(route)
   {}
   Route(SVCG::route_point& start, SVCG::route_point& finish, std::vector<SVCG::route_point>& controlPointList)
      : start(start)
      , finish(finish)
      , controlPointList(controlPointList)
      , route()
   {}
};

struct RouteData
{
   std::vector<Route> landRoutes;
   std::vector<Route> airRoutes;
   RouteData() {}
   RouteData(std::vector<Route>& landRoutes, std::vector<Route>& airRoutes)
      : landRoutes(landRoutes)
      , airRoutes(airRoutes)
   {}
};

struct LightPointData
{
   surface_viewer::FlyZoneAffilation m_fly : 4;
   surface_viewer::GoZoneAffilation m_go : 4;
   float m_x;
   float m_y;
   float m_z;
};

using aff_checker = std::function<bool(std::shared_ptr<SVM::iMatrix<SVCG::route_point>>&, std::shared_ptr<SVM::iMatrix<size_t>>&, size_t, size_t)>;
using height_corrector = std::function<float(float)>;

struct PathFinderLogic
{
   aff_checker affilationChecker;
   height_corrector heightCorrector;
   PathFinderLogic(aff_checker checker, height_corrector corrector)
      : affilationChecker(checker)
      , heightCorrector(corrector)
   {}
};

// Настройки патфайндера...и в принципе всё..
struct PathFinderSettings
{
   bool multithread;

   // NOTE: Это для статистики
   std::vector<size_t> statFieldIndex;    // Статистическое поле - н-мерная матрица статистики

   bool research; // флаг исследовательского пробега
   bool landPath; // необходимость расчета наземного пути
   size_t packetSize;   // размер пакета путей для многопоточного вызова(0 - все задачи в 1 пул)

   PathFinderSettings(bool multithread = true, std::vector<size_t> statFieldIndex = {}, bool research = false, bool landPath = true, size_t packetSize = 0)
      : multithread(multithread)
      , statFieldIndex(statFieldIndex)
      , research(research)
      , landPath(landPath)
      , packetSize(packetSize)
   {}
};

// Статистика для вывода в файл
struct PathFinderStatistic
{

};

class PathFinder
{
   //INST_OBJECT(PathFinder)
public:
   PathFinder();
public:
   ~PathFinder();
protected:
   /*virtual void initPowertrain() override;
   virtual void initMiscellaneous() override;*/
public:
   void FindPath(StrategySettings settings, std::shared_ptr<RouteData>& route/*, const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& rawdata*/, /*bool multithread = true, size_t countIdx = 0, size_t legnthIdx = 0, bool research = false, bool landPath = true, size_t packetSize = 0*/const PathFinderSettings pathFinderSettings, PathFinderStatistic& statistic);
private:
   bool prepareControlPoint(StrategySettings settings, size_t iterations, std::vector<Route>& landRoutes, std::vector<Route>& airRoutes/*, const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& rawdata*/);
   //std::shared_ptr<SVM::iMatrix<size_t>> buildLandCoverage(StrategySettings settings, std::vector<Route>& airRoutes);
   bool landPathCoverage(StrategySettings settings, std::vector<Route>& landRoutes, std::vector<Route>& airRoutes);
   SVCG::route_point checkRetranslateFlyPointAffilation(int row, int col/*, const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& rawdata*/);
   Route findLandPath(Route& route/*, std::shared_ptr<SVM::iMatrix<size_t>>& coverageMatrix*/, bool multithread, bool* pathFounded);
   Route findAirPath(Route& route, size_t iterations, bool multithread);
   std::vector<SVCG::route_point> findPath(SVCG::route_point& start, SVCG::route_point& finish, PathFinderLogic& logic/*, std::shared_ptr<SVM::iMatrix<size_t>>& coverageMatrix*/, bool multithread, bool* pathFound);
   void fillSupportData(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& rawdata*/);
   void clearSupportData();
   //inline void AddItem();
//signals:
   //void TimeUpdate(ProfilingData);
   //void TaskDataUpdate(QVariant);
   //void SubTaskDataUpdate(QVariant);
private:
   //std::shared_ptr<SVM::iMatrix<SurfaceElement>> m_rawdata;
   //size_t m_rowCount, m_colCount;
   //std::shared_ptr<iMatrix> m_rawdata;
   //std::vector<QFuture<Route>> m_funAirVector;
   //std::vector<QFuture<Route>> m_funLandVector;
   //QFutureSynchronizer<Route> m_synchronizer;

   //std::shared_ptr<settings::application_settings> m_appSettings;
   //XFM::Wrapper<LightPointData> m_data;
   //std::shared_ptr<SVM::iMatrix<SurfaceElement>> m_data;
   //XFM::Wrapper<size_t> m_pointScore;

   // NOTE: Опасно!!!
   PathFinderStatistic* m_statistic;

   ExperimentMeta m_vmeta;
   size_t m_rowCount;
   size_t m_colCount;
};
