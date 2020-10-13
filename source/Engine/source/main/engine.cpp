#include "stdafx.h"
#include "engine.h"
//#include "common/matrix.h"
//#include "main/dispatcher.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
//#include "settings/common/settings.h"
#include <windows.h>
#include "algorithm/statistic.h"

using namespace engine;
std::recursive_mutex g_engineMutex;

Engine::Engine(central_pack* pack)
   : Central(pack)
   , m_rawdata(std::make_shared<pathfinder::RoutePointMatrix>(SVCG::route_point{}))
   , m_routedata(std::make_shared<pathfinder::route_data>())
   , m_pathfinder(std::make_unique<pathfinder::PathFinderPipeline>(pack))
{
   /*m_noGoLowLevel = 73.f;
   m_noGoHighLevel = 120.f;
   m_noFlyLevel = 139.f;
   m_minDangerAngle = 40.f;
   m_maxDangerAngle = 70.f;*/
}

void Engine::ProcessPathFind(const ColregSimulation::scenario_data& scenarioData, const pathfinder::GeoMatrix& rawData, std::shared_ptr<settings::application_settings> settings, std::function<void(void)> completeCallback)
{
   m_settings = settings;
   std::thread(&Engine::processPathFind, this, scenarioData, rawData, completeCallback).detach();
}

void Engine::processPathFind(const ColregSimulation::scenario_data& scenarioData, const pathfinder::GeoMatrix& rawData, std::function<void(void)> completeCallback)
{
   // TODO: разобраться с настройками
   convertMap(rawData, m_rawdata);
   processPathFindInternal(scenarioData, pathfinder::path_finder_settings{}, completeCallback);
}

void Engine::processPathFindInternal(const ColregSimulation::scenario_data& scenarioData, pathfinder::path_finder_settings stt, std::function<void(void)> completeCallback)
{
   m_indata = std::make_shared<pathfinder::path_finder_indata>(pathfinder::path_finder_indata{
      scenarioData.unit_data,
      stt,
      pathfinder::path_finder_statistic(),
      pathfinder::strategy_settings{ pathfinder::StrategyType::ST_RHOMBOID, 5. } // NOTE: радиус пока что тут настраивается
   });
   m_pathfinder->FindPath([completeCallback]() { completeCallback(); }, m_rawdata, m_indata);
   //using namespace std::chrono_literals;
   //std::this_thread::sleep_for(3ms);
   //completeCallback();
}

void Engine::convertMap(const pathfinder::GeoMatrix& rawdataSrc, std::shared_ptr<pathfinder::RoutePointMatrix> rawdataDst)
{
   size_t rowCount = rawdataSrc.GetRowCount();
   size_t colCount = rawdataSrc.GetColCount();
   rawdataDst->SetRowCount(rowCount);
   if (rowCount > 0)
      rawdataDst->SetColCount(colCount);
   for (size_t rIdx = 0; rIdx < rowCount; rIdx++)
   {
      for (size_t cIdx = 0; cIdx < colCount; cIdx++)
         rawdataDst->Set(rIdx, cIdx, SVCG::route_point(rIdx, cIdx, rawdataSrc.Get(rIdx, cIdx), checkFlyZone(rawdataSrc, rIdx, cIdx).fza, checkGoZone(rawdataSrc, rIdx, cIdx).gza));
   }
}

pathfinder::check_fly_zone_result Engine::checkFlyZone(const pathfinder::GeoMatrix& rawdataSrc, int rowIdx, int colIdx)
{
   auto& pth_stt = m_settings->pth_stt;
   return { (rawdataSrc.Get(rowIdx, colIdx) > pth_stt.lvl_stt.max_air_height) ? pathfinder::FlyZoneAffilation::FZA_FORBIDDEN : pathfinder::FlyZoneAffilation::FZA_NORMAL };
}

pathfinder::check_go_zone_result Engine::checkGoZone(const pathfinder::GeoMatrix& rawdataSrc, int rowIdx, int colIdx)
{
   // NOTE: стоблцы идут снизу вверх(по крайней мере тут считаем именно так)
   size_t rowCount = rawdataSrc.GetRowCount();
   size_t colCount = rawdataSrc.GetColCount();
   return checkAngles(
        rawdataSrc.Get(rowIdx, colIdx) // center
      , rawdataSrc.Get(rowIdx, colIdx > 0 ? colIdx - 1 : 0) // left
      , rawdataSrc.Get(rowIdx, colIdx < colCount - 1 ? colIdx + 1 : colCount - 1) // right
      , rawdataSrc.Get(rowIdx < rowCount - 1 ? rowIdx + 1 : rowCount - 1, colIdx) // top
      , rawdataSrc.Get(rowIdx > 0 ? rowIdx - 1 : 0, colIdx) // bottom
      , rawdataSrc.Get(rowIdx < rowCount - 1 ? rowIdx + 1 : rowCount - 1, colIdx > 0 ? colIdx - 1 : 0) // topleft
      , rawdataSrc.Get(rowIdx > 0 ? rowIdx - 1 : 0, colIdx > 0 ? colIdx - 1 : 0) // bottomleft
      , rawdataSrc.Get(rowIdx < rowCount - 1 ? rowIdx + 1 : rowCount - 1, colIdx < colCount - 1 ? colIdx + 1 : colCount - 1) // topright
      , rawdataSrc.Get(rowIdx > 0 ? rowIdx - 1 : 0, colIdx > 0 ? colIdx - 1 : 0) // bottomright
   );
}

pathfinder::check_go_zone_result Engine::checkAngles(double center, double left, double right, double top, double bottom, double topleft, double bottomleft, double topright, double bottomright)
{
   auto& pth_stt = m_settings->pth_stt;
   // NOTE: проверяем 4 направления по 8 сторонам света
   pathfinder::check_go_zone_result result;
   double cellMult = 6.;
   float mult = 180.f/static_cast<float>(M_PI);
   // top-bottom
   float angleTB = (atan(fabs(top - center) / cellMult)*mult + atan(fabs(center - bottom) / cellMult)*mult)/2.;
   // left-right
   float angleLR = (atan(fabs(left - center) / cellMult)*mult + atan(fabs(center - right) / cellMult)*mult)/2.;
   // topleft-bottomright
   float angleTLBR = (atan(fabs(topleft - center) / cellMult) * mult + atan(fabs(center - bottomright) / cellMult) * mult) / 2.;
   // topright-bottomleft
   float angleTRBL = (atan(fabs(bottomleft - center) / cellMult) * mult + atan(fabs(center - topright) / cellMult) * mult) / 2.;
   result.asn = angleTB;
   result.awe = angleLR;
   result.aswne = angleTRBL;
   result.asenw = angleTLBR;

   auto minDA = pth_stt.lvl_stt.dangerous_land_angle;
   auto maxDA = pth_stt.lvl_stt.max_land_angle;
   auto minLH = pth_stt.lvl_stt.min_land_height;
   auto maxLH = pth_stt.lvl_stt.max_land_height;
   //qDebug() << "angles:" << fabs(angleUD) << fabs(angleLR);
   bool maxAngleExcess = (maxDA < fabs(angleTB)) || (maxDA < fabs(angleLR)) || (maxDA < fabs(angleTRBL)) || (maxDA < fabs(angleTLBR));
   bool minAngleExcess = (minDA < fabs(angleTB)) || (minDA < fabs(angleLR)) || (minDA < fabs(angleTRBL)) || (minDA < fabs(angleTLBR));
   if (maxAngleExcess
       || (center < minLH)
       || (center > maxLH))
      result.gza = pathfinder::GoZoneAffilation::GZA_FORBIDDEN;
   else if (minAngleExcess)
      result.gza = pathfinder::GoZoneAffilation::GZA_DANGEROUS;
   else
      result.gza = pathfinder::GoZoneAffilation::GZA_NORMAL;
   return result;
}

void Engine::LaunchResearch(std::function<void(void)> callback)
{
   m_endRoundCallback = callback;
   auto& res_stt = m_settings->res_stt;
   generateResMap(res_stt.map_size);
   switch (res_stt.res_type)
   {
   case settings::ResearchType::RT_TIME:
   {
      timeResearch();
      break;
   }
   case settings::ResearchType::RT_LENGTH:
   {
      lengthResearch();
      break;
   }
   case settings::ResearchType::RT_THREAD:
   {
      threadResearch();
      break;
   }
   }
}

void Engine::timeResearch(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>&, std::shared_ptr<ResearchResultGen1>& result*/)
{
   //const auto& pathfinder = WFM::GetSharedInstance<PathFinder>(DBG_DATA);
   //auto rawmap = WFM::CreateSharedObject<SVM::iMatrix<SurfaceElement>>();
   //// TODO: разобраться, нужна ли вообще карта?...может просто генератор сделать?..
   //const auto& resStt = m_appSettings->res_settings.g1s;
   //const auto& cntRange = resStt.countRange;
   //const auto& lnRange = resStt.lengthRange;
   //size_t cntCnt = 0, lnCnt = 0;
   //for (size_t count = cntRange.min; count <= cntRange.max; count += cntRange.step)
   //   cntCnt++;
   //for (double length = lnRange.min; length <= lnRange.max; length += lnRange.step)
   //   lnCnt++;
   //result->dataStorage->Prepare(cntCnt, lnCnt);
   ////WFM::GetSharedInstance<RuntimeStatistic>(DBG_DATA)->Prepare(cntCnt, lnCnt);
   //generateResMap(rawmap, resStt);
   //size_t cntIdx = 0, lnIdx = 0;
   ////__int64 ctr1 = 0, ctr2 = 0, freq = 0;
   //const RoutePoint startRP{ rawmap->Get(2, 2).x, rawmap->Get(2, 2).y, rawmap->Get(2, 2).z };
   //const MatrixPoint startMP{ 2, 2 };
   //// NOTE: расстояние на клетку
   //const double step = static_cast<double>(fabs(rawmap->Get(0, 0).x - rawmap->Get(1, 0).x));
   //CombinedPoint startPoint{ startRP, startMP };
   //double fcntstep = 100./cntCnt;
   //double flnstep = fcntstep/lnCnt;
   //for (size_t count = cntRange.min; count <= cntRange.max; count += cntRange.step)
   //{
   //   lnIdx = 0;
   //   for (double length = lnRange.min; length <= lnRange.max; length += lnRange.step)
   //   {          
   //      std::shared_ptr<RouteData> sroute = WFM::CreateSharedObject<RouteData>();
   //      std::shared_ptr<RouteData> mroute = WFM::CreateSharedObject<RouteData>();
   //      std::vector<CombinedPoint> controlPoints{};
   //      auto fnIdx = static_cast<size_t>(round(length/step/2.));
   //      auto fnIdxRow = fnIdx < rawmap->GetRowCount() ? fnIdx : rawmap->GetRowCount() - 1;
   //      auto fnIdxCol = fnIdx < rawmap->GetColCount() ? fnIdx : rawmap->GetColCount() - 1;
   //      const RoutePoint finishRP{rawmap->Get(fnIdxRow, fnIdxCol).x, rawmap->Get(fnIdxRow, fnIdxCol).y, rawmap->Get(fnIdxRow, fnIdxCol).z};
   //      const MatrixPoint finishMP{fnIdxRow, fnIdxCol};
   //      CombinedPoint finishPoint{finishRP, finishMP};
   //      sroute->landRoutes.emplace_back(Route(startPoint, finishPoint, controlPoints));
   //      for (size_t idx = 0; idx < count; idx++)
   //         sroute->airRoutes.emplace_back(Route(startPoint, finishPoint, controlPoints));
   //      mroute->landRoutes.emplace_back(Route(startPoint, finishPoint, controlPoints));
   //      for (size_t idx = 0; idx < count; idx++)
   //         mroute->airRoutes.emplace_back(Route(startPoint, finishPoint, controlPoints));
   //      qDebug() << "engine res iterations:" << count << length;
   //      //qint64 startTime = CURTIME_MS();
   //      if (resStt.multiThreadTest)
   //      {
   //         for (size_t idx = 0; idx < resStt.iterCount; idx++)
   //         {
   //            PathFinderStatistic statistic;
   //            pathfinder->FindPath(StrategySettings{ST_RHOMBOID, 1000.f}, mroute, rawmap, PathFinderSettings{true, {cntIdx, lnIdx}, true, false}, statistic);
   //         }
   //      }
   //      //qint64 finishTime = CURTIME_MS();
   //      //auto arCount = route->airRoutes.back();
   //      //qint64 time = finishTime - startTime;
   //      BaseGeometryElement multi{ static_cast<float>(count), static_cast<float>(0), static_cast<float>(length) };

   //      //startTime = CURTIME_MS();
   //      if (resStt.singleThreadTest)
   //      {
   //         for (size_t idx = 0; idx < resStt.iterCount; idx++)
   //         {
   //            PathFinderStatistic statistic;
   //            pathfinder->FindPath(StrategySettings{ST_RHOMBOID, 1000.f}, sroute, rawmap, PathFinderSettings{false, {cntIdx, lnIdx}, true, false}, statistic);
   //         }
   //      }
   //      //finishTime = CURTIME_MS();
   //      //time = finishTime - startTime;         
   //      BaseGeometryElement single{ static_cast<float>(count), static_cast<float>(0), static_cast<float>(length) };

   //      //qDebug() << startTime << finishTime << sqrt(pow(arCount.start.rp.z - arCount.finish.rp.z, 2.) + pow(arCount.start.rp.x - arCount.finish.rp.x, 2.));
   //      /*qDebug() << "Start time = "
   //               << startTime
   //               << "Finish time = "
   //               << finishTime
   //               << "| start/finish matrix indexes = " << arCount.start.mp.row << arCount.start.mp.col << arCount.finish.mp.row << arCount.finish.mp.col << "-"
   //               << "- path length = " << fabs(arCount.finish.mp.row - arCount.start.mp.row) + fabs(arCount.finish.mp.col - arCount.start.mp.col) << "|"
   //               << "| start/finish coordinates = " << arCount.start.rp.x << arCount.start.rp.z << arCount.finish.rp.x << arCount.finish.rp.z << "-"
   //               << "- radius = " << sqrt(pow(arCount.start.rp.z - arCount.finish.rp.z, 2.) + pow(arCount.start.rp.x - arCount.finish.rp.x, 2.)) << "|";*/
   //      /*qDebug() << startTime
   //               << finishTime
   //               << finishTime - startTime
   //               << sqrt(pow(arCount.start.rp.z - arCount.finish.rp.z, 2.) + pow(arCount.start.rp.x - arCount.finish.rp.x, 2.))
   //               << fabs(arCount.finish.mp.row - arCount.start.mp.row) + fabs(arCount.finish.mp.col - arCount.start.mp.col);*/
   //      //qDebug() << finishTime - startTime;

   //      //resmap->Set(cntIdx, lnIdx, ResearchResultElement(single, multi));
   //      lnIdx++;
   //      emit percent(static_cast<int>(fcntstep*cntIdx + flnstep*lnIdx));
   //      //WFM::GetSharedInstance<StatusWnd>(DBG_DATA)->SetPercent(static_cast<int>(fcntstep*cntIdx + flnstep*lnIdx));
   //   }
   //   cntIdx++;
   //}
   ////Q_ASSERT(resmap->GetRowCount() == cntIdx && resmap->GetColCount() == lnIdx);
}

void Engine::generateResMap(size_t mapSize)
{
   // NOTE: тут всегда что-то квадратное
   m_rawdata->SetRowCount(mapSize);
   m_rawdata->SetColCount(mapSize);
   for (size_t rowIdx = 0; rowIdx < mapSize; rowIdx++)
   {
      for (size_t colIdx = 0; colIdx < mapSize; colIdx++)
      {
         m_rawdata->Set(rowIdx, colIdx, { static_cast<int>(rowIdx), static_cast<int>(colIdx), 0. });
      }
   }
}

void Engine::lengthResearch(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& rawmap, std::shared_ptr<ResearchResultGen2>& result*/)
{
   //Route landRoute;
   //auto routeConvert = [](Route& route, STT::PointSettingElement& setting)
   //{
   //    route = Route(setting.start, setting.finish, setting.controlPointList);
   //};
   //// TODO: как-то скомпоновать
   //auto pointSettings = m_appSettings->sim_settings.pointSettings;
   //routeConvert(landRoute, pointSettings.landSettings.at(0));
   ////routeConvert(m_route->airRoutes, pointSettings.airSettings);

   //const auto& pathfinder = WFM::GetSharedInstance<PathFinder>(DBG_DATA);
   //const auto& resStt = m_appSettings->res_settings.g2s;
   //const auto& cntRange = resStt.countRange;
   //const auto& radRange = resStt.radiusRange;
   //size_t cntCnt = 0, rdCnt = 0;
   //for (size_t count = cntRange.min; count <= cntRange.max; count += cntRange.step)
   //   cntCnt++;
   //for (double length = radRange.min; length <= radRange.max; length += radRange.step)
   //   rdCnt++;
   //result->dataStorage->Prepare(cntCnt, rdCnt);
   //WFM::GetSharedInstance<RuntimeStatistic>(DBG_DATA)->Prepare(cntCnt, rdCnt);

   //size_t cntIdx = 0, rdIdx = 0;

   //double fcntstep = 100./cntCnt;
   //double frdstep = fcntstep/rdCnt;
   //std::shared_ptr<RouteData> sroute = WFM::CreateSharedObject<RouteData>();
   //std::shared_ptr<RouteData> mroute = WFM::CreateSharedObject<RouteData>();

   //auto resRunner = [&rawmap, pathfinder, landRoute, resStt](std::shared_ptr<RouteData> route, BaseGeometryElement& elem, StrategyType type, size_t count, int radius, bool multi, size_t cntIdx, size_t rdIdx)->void
   //{
   //   route->landRoutes.clear();
   //   route->airRoutes.clear();
   //   route->landRoutes.emplace_back(landRoute);
   //   route->airRoutes.resize(count);

   //   for (size_t idx = 0; idx < resStt.iterCount; idx++)
   //   {
   //      PathFinderStatistic statistic;
   //      pathfinder->FindPath(StrategySettings{type, static_cast<double>(radius)}, route, rawmap, PathFinderSettings{multi, {cntIdx, rdIdx}, false}, statistic);
   //   }
   //   //double avgLength = .0;
   //   int length = 0;
   //   for (size_t idx = 0; idx < route->airRoutes.size(); idx++)
   //      length += route->airRoutes.at(idx).route.size();
   //   //avgLength = static_cast<double>(length)/static_cast<double>(route->airRoutes.size());
   //   elem = BaseGeometryElement{ static_cast<float>(count), static_cast<float>(/*avgLength*/length), static_cast<float>(radius) };
   //};

   //for (size_t count = cntRange.min; count <= cntRange.max; count += cntRange.step)
   //{      
   //   rdIdx = 0;
   //   for (int radius = radRange.min; radius <= radRange.max; radius += radRange.step)
   //   {
   //      //qDebug() << "step" << count << radius;
   //      //for (size_t idx = 0; idx < count; idx++)
   //         //sroute->airRoutes.emplace_back(Route(startPoint, finishPoint, controlPoints));

   //      //for (size_t idx = 0; idx < count; idx++)
   //         //mroute->airRoutes.emplace_back(Route(startPoint, finishPoint, controlPoints));
   //      BaseGeometryElement multiRhomboid, singleRhomboid;
   //      BaseGeometryElement multiSnake, singleSnake;
   //      BaseGeometryElement multiSector, singleSector;

   //      if (resStt.multiThreadTest)
   //      {
   //         resRunner(mroute, multiRhomboid, ST_RHOMBOID, count, radius, true, cntIdx, rdIdx);
   //         resRunner(mroute, multiSnake, ST_SNAKE, count, radius, true, cntIdx, rdIdx);
   //         resRunner(mroute, multiSector, ST_SECTOR, count, radius, true, cntIdx, rdIdx);
   //      }
   //      /*if (resStt.singleThreadTest)
   //      {
   //         resRunner(sroute, singleRhomboid, ST_RHOMBOID, count, radius, false, cntIdx, rdIdx);
   //         resRunner(sroute, singleSnake, ST_SNAKE, count, radius, false, cntIdx, rdIdx);
   //         resRunner(sroute, singleSector, ST_SECTOR, count, radius, false, cntIdx, rdIdx);
   //      }*/

   //      result->dataStorage->Set(/*ResearchResultElement(singleRhomboid, multiRhomboid, singleSnake, multiSnake, singleSector, multiSector)*/0.f, cntIdx, rdIdx);
   //      rdIdx++;
   //      emit percent(static_cast<int>(fcntstep*cntIdx + frdstep*rdIdx));
   //   }
   //   cntIdx++;
   //}
}

// NOTE: Исследование направлено на определение оптимального соотношения размера пула задач к расчету и количества потоков
void Engine::threadResearch(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& resmap, std::shared_ptr<ResearchResultGen3>& result*/)
{
   auto& res_stt = m_settings->res_stt;
   // Длин путей 62, 125, 250, 500
   // Потоков 1, 2, 4, 8
   // Пул задач 2, 4, 8
   // Путей 2, 4, 8, 16, 32, 64, 128

   for (size_t lengthIdx = 0; lengthIdx < res_stt.length_range.values.size(); lengthIdx++)
   {
      double length = res_stt.length_range.values.at(lengthIdx);
      for (size_t threadPoolIdx = 0; threadPoolIdx < res_stt.thread_pool_range.values.size(); threadPoolIdx++)
      {
         size_t threadCount = res_stt.thread_pool_range.values.at(threadPoolIdx);
         for (size_t taskPoolIdx = 0; taskPoolIdx < res_stt.task_pool_range.values.size(); taskPoolIdx++)
         {
            size_t taskCount = res_stt.task_pool_range.values.at(taskPoolIdx);
            for (size_t flyCountIdx = 0; flyCountIdx < res_stt.fly_count_range.values.size(); flyCountIdx++)
            {
               size_t flyCount = res_stt.fly_count_range.values.at(flyCountIdx);
               m_threadResStorage.data.emplace_back(ThreadResearchComplexStorage::SuperCell{
                  ThreadResearchComplexStorage::SuperCell::Index{
                     threadPoolIdx,
                     taskPoolIdx,
                     flyCountIdx,
                     lengthIdx,
                     threadCount,
                     taskCount,
                     flyCount,
                     length
                  },
                  ThreadResearchComplexStorage::SuperCell::Result{
                     0
                  }
               });
            }
         }
      }
   }
   m_threadResStorage.info = { 
      res_stt.thread_pool_range,
      res_stt.task_pool_range,
      res_stt.fly_count_range,
      res_stt.length_range
   };
   m_threadTaskCurrentIdx = 0;
   threadResNextStep();
}

void Engine::threadResNextStep()
{
   ATLASSERT(false);
   std::lock_guard<std::recursive_mutex> guard(g_engineMutex);
   __int64 startTime;
   CURTIME_MS(startTime);
   if (m_threadTaskCurrentIdx > 0)
   {
      m_threadResStorage.data.at(m_threadTaskCurrentIdx - 1).result.time.finish = startTime;
      m_threadResStorage.data.at(m_threadTaskCurrentIdx - 1).result.time.apply();
   }
   if (m_threadTaskCurrentIdx >= m_threadResStorage.data.size())
   {
      m_endRoundCallback();
      //m_logger->LogThreadResearchResult(L"", GetThreadResearchResult());
      return;
   }
   auto& res_stt = m_settings->res_stt;
   auto& threadRes = m_threadResStorage.data.at(m_threadTaskCurrentIdx);
   auto& threadResIndex = threadRes.index;
   threadRes.result.time.start = startTime;
   ColregSimulation::scenario_data data;
   generateResScenarioData(data, res_stt, threadResIndex);
   pathfinder::path_finder_settings stt(true, {}, true, true, 0, 0, false);
   stt.packet_size = threadResIndex.task_pool_value;
   stt.thread_count = threadResIndex.thread_pool_value;
   GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "task started: [fly: %i, length: %f, task: %i, thread: %i]", threadResIndex.fly_count_value, threadResIndex.length_value, threadResIndex.task_pool_value, threadResIndex.thread_pool_value);
   std::thread(&Engine::processPathFindInternal, this, data, stt, [this]() { threadResNextStep(); }).detach();
   //m_communicator->Message(ICommunicator::MS_Debug, "Thread task idx %i", m_threadTaskCurrentIdx);
   m_threadTaskCurrentIdx++;
   GetPack()->comm->SetProgress(static_cast<unsigned int>(static_cast<double>(m_threadTaskCurrentIdx)/static_cast<double>(m_threadResStorage.data.size())*100.));
}

void Engine::generateResScenarioData(ColregSimulation::scenario_data& data, const settings::research_settings& stt, const ThreadResearchComplexStorage::SuperCell::Index& idx)
{
   data.unit_data.air_units.resize(idx.fly_count_value);
   data.unit_data.land_units.resize(1);
   int mapSize = static_cast<int>(stt.map_size);
   int curSize = static_cast<int>(idx.length_value);
   // NOTE: индекс предельной точки отличается от размера карты на 1
   int fnCoord = curSize < mapSize - 1 ? curSize : mapSize - 1;
   for (auto& elem : data.unit_data.air_units)
   {
      elem.start = SVCG::route_point{ 0, 0 };
      elem.finish = SVCG::route_point{ fnCoord, fnCoord };
   }
   for (auto& elem : data.unit_data.land_units)
   {
      elem.start = SVCG::route_point{ 0, 0 };
      elem.finish = SVCG::route_point{ fnCoord, fnCoord };
   }
}

void Engine::logThreadResearchResult()
{

}

engine::iEngine* CreateEngine(central_pack* pack)
{
   return new engine::Engine(pack);
}