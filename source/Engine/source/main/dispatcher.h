#pragma once

//#include "base/instance.h"
#include <memory>
#include <string>
#include "colreg/BaseRef.h"
#include "colreg/ModuleGuard.h"
#include "algorithm\pathfinder\pathfinder.h"
#include "common\file_storage.h"
#include "engine.h"

class Dispatcher : public Singleton<Dispatcher>
{
public:
   Dispatcher(/*GlViewerDlg::ViewerCluster cluster
            , std::shared_ptr<VisualizationWnd> visulizationWnd
            , std::shared_ptr<PathfindingWnd> pathfindingWnd
            , std::shared_ptr<SystemWnd> m_systemWnd*/);
   ~Dispatcher();
private:
   void recountConfigNavigation();
   void preparePathfindingView();
   void onScenarioLoad();
public:
   void LoadScenario(const wchar_t*);

   const std::shared_ptr<settings::application_settings> GetSettings();
   //std::shared_ptr<SVM::iMatrix<SurfaceElement>> GetRawMap();
protected:
   void OnArrayReset();
   void OnArrayResetGen2();
   void OnArrayResetGen3();

   void loadHeightMap(const char* name);
   void loadResearch();

   void handleGen1ResFinished();
   void handleGen2ResFinished();
   void handleGen3ResFinished();
public:
   void Simulate();
   void FindPath();
   void TimeResearch();
   void MatlabDump();
private:
   //QtMapViewer m_qtMapViewer;
   //QtResViewer m_qtResViewer;
   //GlMapViewer m_glMapViewer;
   //GlResViewer m_glResViewer;
   //Gl2DMapViewer m_gl2dMapViewer;
   //Gl2DResViewer m_gl2dResViewer;

   //std::unique_ptr<LoadingDlg> m_loadingdlg;

   //std::unique_ptr<RuntimeStatistic> m_stat;

   //std::unique_ptr<VisualizationWnd> m_visulizationWnd;
   //std::unique_ptr<PathfindingWnd> m_pathfindingWnd;
   //std::unique_ptr<SystemWnd> m_systemWnd;

   //std::unique_ptr<DomSettings> m_dom;
   std::shared_ptr<file_utils::global_path_storage> m_fileStorage;

   std::unique_ptr<engine::Engine> m_engine;

   //QFutureWatcher<void> m_resWatcher;

   //std::unique_ptr<QImage> m_layerHMap;
   //std::unique_ptr<QHeightMapSurfaceDataProxy> m_mapLayerProxy;

   //std::unique_ptr<SVM::iMatrix<SurfaceElement>> m_rawmap;
   //std::unique_ptr<ResearchResultGen1> m_resmapGen1;
   //std::unique_ptr<ResearchResultGen2> m_resmapGen2;
   //std::unique_ptr<ResearchResultGen3> m_resmapGen3;

   //std::unique_ptr<RouteData> m_route;
   std::unique_ptr<pathfinder::PathFinder> m_pathfinder;
   

   //std::map<float, QColor> m_colorPercentMap;
};

/*namespace dispatcher
{
   Dispatcher* getDispatcher();
   void start(const char*);
}*/