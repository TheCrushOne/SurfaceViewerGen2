#include "stdafx.h"
#include "dispatcher.h"

#include "engine.h"
#include "algorithm/pathfinder/pathfinder.h"
#include "common/settings.h"
#include "crossdllinterface\SettingsSerializerInterface.h"
#include "crossdllinterface\UnitDataSerializerInterface.h"
#include "common/file_storage.h"
#include "common/utils.h"

#include <direct.h>
//#include "settings/xml/domsettings.h"

//#include "ui/mainwnd.h"
//
//#include "ui/datavis/qt/qtmapviewer.h"
//#include "ui/datavis/qt/qtresviewer.h"
//#include "ui/datavis/opengl/glmapviewer.h"
//#include "ui/datavis/opengl/glresviewer.h"
//#include "ui/datavis/opengl2d/gl2dmapviewer.h"
//#include "ui/datavis/opengl2d/gl2dresviewer.h"
//#include "ui/dialog/loadingdlg.h"
//
//#include "settings/common/settings.h"
//
//#include "ui/dockable/pathfindingwnd.h"
//#include "ui/dockable/systemwnd.h"
//#include "ui/dockable/visualizationwnd.h"
//
//#include "algorithm/statistic.h"
//#include "dumper/statisticdumper.h"


//using namespace QtConcurrent;

Dispatcher::Dispatcher(/*GlViewerDlg::ViewerCluster cluster
                     , std::shared_ptr<VisualizationWnd> visulizationWnd
                     , std::shared_ptr<PathfindingWnd> pathfindingWnd
                     , std::shared_ptr<SystemWnd> m_systemWnd*/)
   /*: m_qtMapViewer(cluster.m_hmapviewer)
   , m_qtResViewer(cluster.m_rresviewer)
   , m_glMapViewer(cluster.m_glmapviewer)
   , m_glResViewer(cluster.m_glresviewer)
   , m_gl2dMapViewer(cluster.m_gl2dmapviewer)
   , m_gl2dResViewer(cluster.m_gl2dresviewer)
   , m_visulizationWnd(visulizationWnd)
   , m_pathfindingWnd(pathfindingWnd)
   , m_systemWnd(m_systemWnd)*/
{
   //m_appSettings = WFM::CreateSharedObject<settings::application_settings>();
   //m_loadingdlg = WFM::CreateSharedInstance<LoadingDlg>(DBG_DATA);
   //m_stat = WFM::CreateSharedInstance<RuntimeStatistic>(DBG_DATA);
   
   m_appSettings = std::make_unique<settings::application_settings>();
   m_engine = std::make_unique<engine::Engine>();
   m_pathfinder = std::make_unique<pathfinder::PathFinder>();
   
   //createUnitDataSerializer();
   //createSettingsSerializer();
   //m_dom = WFM::CreateSharedObject<DomSettings>();
}

Dispatcher::~Dispatcher()
{}

const std::shared_ptr<settings::application_settings> Dispatcher::GetSettings()
{
   return m_appSettings;
}

//std::shared_ptr<SVM::iMatrix<SurfaceElement>> Dispatcher::GetRawMap()
//{
//   return m_rawmap;
//}

void Dispatcher::LoadScenario(const wchar_t* name)
{
   m_fileStorage = std::make_shared<file_utils::heightmap_file_storage>(name);
   
   loadHeightMap(SVGUtils::wstringToString(m_fileStorage->map_path).c_str());
}

void Dispatcher::onScenarioLoad()
{
   //m_engine->OnScenarioLoad();
   //m_qtMapViewer->OnScenarioLoad();
   //m_qtResViewer->OnScenarioLoad();
   //m_glMapViewer->OnScenarioLoad();
   //m_glResViewer->OnScenarioLoad();
   //m_gl2dMapViewer->OnScenarioLoad();
   //m_gl2dResViewer->OnScenarioLoad();
   //m_visulizationWnd->OnScenarioLoad();
   //m_pathfindingWnd->OnScenarioLoad();
   //m_systemWnd->OnScenarioLoad();
}

// TODO: дописать!!!
void Dispatcher::loadHeightMap(const char* name)
{
   //auto fileinfo = m_appSettings->sim_settings.fileinfo;
   //QString configName = fileinfo.path + "/" + fileinfo.base + ".xml";
   //m_dom->SetMapMetaFileName(configName);
   //m_dom->Read(ST_MAPMETA, m_appSettings);
   //m_pathfindingWnd->ResetScenarioSettings(m_appSettings->sim_settings);
   //m_route = WFM::CreateSharedObject<RouteData>();

   //m_layerHMap = WFM::CreateSharedObject<QImage>(fileinfo.full);
   //m_mapLayerProxy = WFM::CreateSharedObject<QHeightMapSurfaceDataProxy>();
   //connect(m_mapLayerProxy.get(), &QHeightMapSurfaceDataProxy::arrayReset,
   //        this, &Dispatcher::OnArrayReset);
   //m_mapLayerProxy->setValueRanges(34.0f, 40.0f, 18.0f, 24.0f);
   //m_mapLayerProxy->setHeightMap(*m_layerHMap.get());
   //m_colorPercentMap = {
   //   { 0.0, Qt::black },
   //   { 0.38, Qt::darkYellow },
   //   { 0.39, Qt::darkGreen },
   //   { 0.5, Qt::darkGray },
   //   { 1.0, Qt::gray },
   //};
}

void Dispatcher::loadResearch()
{
   //m_dom->SetResMetaFileName(m_appSettings->res_settings.fileinfo.full);
   //m_dom->Read(ST_RESMETA, m_appSettings);
}

void Dispatcher::Simulate()
{
   //connect(m_mapLayerProxy.get(), &QHeightMapSurfaceDataProxy::arrayReset,
   //        this, &Dispatcher::OnArrayReset);
   //m_qtMapViewer->Simulate(m_mapLayerProxy, m_rawmap, m_colorPercentMap);
   //m_glMapViewer->Simulate(m_mapLayerProxy, m_rawmap, m_colorPercentMap);
   //m_gl2dMapViewer->Simulate(m_mapLayerProxy, m_rawmap, m_colorPercentMap);
   //// WARNING: иногда тут падает
   ////emit m_mapLayerProxy->arrayReset();
   //recountConfigNavigation();
   //preparePathfindingView();
}

void Dispatcher::FindPath()
{
   //auto routeConvert = [](std::vector<Route>& routes, std::vector<STT::PointSettingElement>& settings)
   //{
   //    for (auto& setting : settings)
   //       routes.emplace_back(Route(setting.start, setting.finish, setting.controlPointList));
   //};
   //// TODO: как-то скомпоновать
   //auto pointSettings = m_appSettings->sim_settings.pointSettings;
   //routeConvert(m_route->landRoutes, pointSettings.landSettings);
   //routeConvert(m_route->airRoutes, pointSettings.airSettings);

   //PathFinderStatistic statistic;
   //m_pathfinder->FindPath(m_appSettings->sim_settings.strategySettings, m_route, m_rawmap, PathFinderSettings{}, statistic);
   //m_qtMapViewer->ShowPath(m_route);
   //m_glMapViewer->ShowPath(m_route);
}

void Dispatcher::TimeResearch()
{
   //if (m_appSettings->res_settings.type == STT::ResearchType::RT_GEN1)
   //{
   //   m_loadingdlg->show();
   //   m_resmapGen1->dataStorage = WFM::CreateSharedObject<SVM::iHyperCell<float>>(2);
   //   connect(&m_resWatcher, &QFutureWatcher<void>::finished, this, &Dispatcher::handleGen1ResFinished);
   //   QFuture<void> res = run(m_engine.get(), &Engine::TimeResearchGen1, m_rawmap, m_resmapGen1);
   //   //m_engine->TimeResearch(m_resmap, m_rawmap);
   //   m_resWatcher.setFuture(res);
   //}
   //else if (m_appSettings->res_settings.type == STT::ResearchType::RT_GEN2)
   //{
   //   m_loadingdlg->show();
   //   m_layerHMap = WFM::CreateSharedObject<QImage>(m_appSettings->res_settings.fileinfo.path + "/" + m_appSettings->res_settings.g2s.mapName + ".png");
   //   m_mapLayerProxy = WFM::CreateSharedObject<QHeightMapSurfaceDataProxy>();
   //   connect(m_mapLayerProxy.get(), &QHeightMapSurfaceDataProxy::arrayReset,
   //           this, &Dispatcher::OnArrayResetGen2);
   //   m_mapLayerProxy->setValueRanges(34.0f, 40.0f, 18.0f, 24.0f);
   //   m_mapLayerProxy->setHeightMap(*m_layerHMap.get());
   //   m_colorPercentMap = {
   //      { 0.0, Qt::black },
   //      { 0.38, Qt::darkYellow },
   //      { 0.39, Qt::darkGreen },
   //      { 0.5, Qt::darkGray },
   //      { 1.0, Qt::gray },
   //   };
   //}
   //else if (m_appSettings->res_settings.type == STT::ResearchType::RT_GEN3)
   //{
   //   m_loadingdlg->show();
   //   m_resmapGen3->dataStorage = WFM::CreateSharedObject<SVM::iHyperCell<float>>(3);
   //   //m_resmap = WFM::CreateSharedObject<SVM::iMatrix<ResearchResultElement>>();
   //   connect(&m_resWatcher, &QFutureWatcher<void>::finished, this, &Dispatcher::handleGen3ResFinished);
   //   QFuture<void> res = run(m_engine.get(), &Engine::ThreadResearchGen3, m_rawmap, m_resmapGen3);
   //   m_resWatcher.setFuture(res);
   //}
}

void Dispatcher::handleGen1ResFinished()
{
   ////m_loadingdlg->hide();
   //auto dump = m_stat->GetStatDump();
   //m_qtResViewer->LoadResult(dump.meta, dump.task);
   //m_glResViewer->LoadResult(dump.meta, dump.task);
   //m_gl2dResViewer->LoadResult(dump.meta, dump.task);
}

void Dispatcher::handleGen2ResFinished()
{
   //m_loadingdlg->hide();

   //auto dump = m_stat->GetStatDump();
   //m_qtResViewer->LoadResult(dump.meta, dump.task);
   //m_glResViewer->LoadResult(dump.meta, dump.task);
   //m_gl2dResViewer->LoadResult(dump.meta, dump.task);
}

void Dispatcher::handleGen3ResFinished()
{
   //m_loadingdlg->hide();
}

void Dispatcher::MatlabDump()
{
   //WFM::GetSharedInstance<StatisticDumper>(DBG_DATA)->Dump(m_appSettings, WFM::GetSharedInstance<Statistic>(DBG_DATA)->GetStatDump(), m_resmap);
}

void Dispatcher::OnArrayReset()
{
   //m_rawmap = WFM::CreateSharedObject<SVM::iMatrix<SurfaceElement>>();
   //m_engine->ConvertMap(m_mapLayerProxy, m_rawmap, m_appSettings);

   //onScenarioLoad();
}

void Dispatcher::OnArrayResetGen2()
{
   // NOTE: исследование, генерация 2
   //QString configName = m_appSettings->res_settings.fileinfo.path + "/" + m_appSettings->res_settings.g2s.mapName + ".xml";
   //m_appSettings->sim_settings.fileinfo.full = m_appSettings->res_settings.fileinfo.path + "/" + m_appSettings->res_settings.g2s.mapName + ".xml";
   //m_appSettings->sim_settings.fileinfo.base = m_appSettings->res_settings.g2s.mapName;
   //m_appSettings->sim_settings.fileinfo.path = m_appSettings->res_settings.fileinfo.path;
   //auto fileinfo = m_appSettings->sim_settings.fileinfo;
   //m_dom->SetMapMetaFileName(configName);
   //m_dom->Read(ST_MAPMETA, m_appSettings);

   //m_rawmap = WFM::CreateSharedObject<SVM::iMatrix<SurfaceElement>>();
   //m_engine->ConvertMap(m_mapLayerProxy, m_rawmap, m_appSettings);

   //m_resmapGen2->dataStorage = WFM::CreateSharedObject<SVM::iHyperCell<float>>(2);
   //connect(&m_resWatcher, &QFutureWatcher<void>::finished, this, &Dispatcher::handleGen2ResFinished);
   //QFuture<void> res = run(m_engine.get(), &Engine::LengthResearchGen2, m_rawmap, m_resmapGen2);
   ////m_engine->TimeResearch(m_resmap, m_rawmap);
   //m_resWatcher.setFuture(res);
}

void Dispatcher::OnArrayResetGen3()
{
   // NOTE: полуавтомат
   // NOTE: исследование, генерация 3
   //connect(&m_resWatcher, &QFutureWatcher<void>::finished, this, &Dispatcher::handleGen3ResFinished);
}

// Пересчет конфига с клеточных на абсолютные
void Dispatcher::recountConfigNavigation()
{
   //auto matr = m_mapLayerProxy.get();
   //if (matr)
   //   return;
   //auto fillRp = [matr](CombinedPoint& cp)
   //{
   //   auto& mp = cp.mp;
   //   auto& rp = cp.rp;
   //   auto pt = matr->itemAt(mp.row, mp.col);
   //   rp.x = pt->x();
   //   rp.y = pt->y();
   //   rp.z = pt->z();
   //};
   //auto processSettings = [fillRp](std::vector<STT::PointSettingElement>& settings)
   //{
   //   for (auto& unit : settings)
   //   {
   //      fillRp(unit.start);
   //      fillRp(unit.finish);
   //      for (auto& controlPoint : unit.controlPointList)
   //         fillRp(controlPoint);
   //   };
   //};
   //auto pointSettings = GetSettings()->sim_settings.pointSettings;
   //processSettings(pointSettings.landSettings);
   //processSettings(pointSettings.airSettings);
}

void Dispatcher::preparePathfindingView()
{
   //m_pathfindingWnd->ResetPathfindingModel(m_appSettings->sim_settings.pointSettings);
}

/*namespace dispatcher
{
   Dispatcher _dispatcher;

   Dispatcher* GetDispatcher() { return &_dispatcher; }

   void Start(const char* filename) {}
   
   void Stop() {}
}*/