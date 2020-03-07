#include "stdafx.h"
#include "pathfinder.h"
#include <math.h>
#include <limits.h>
#include <future>
//#include "framework/base/instance.h"
//#include "common/matrix.h"
//#include "main/engine.h"
//#include "main/dispatcher.h"
//#include "settings/common/settings.h"



#define THREADDEBUG(line) qDebug() << line << "thread:" << QThread::currentThreadId();

using namespace pathfinder;

PathFinder::PathFinder()
   //: m_data(LightPointData())
   //: m_pointScore(0)
   : m_rowCount(0)
   , m_colCount(0)
   , m_statistic(nullptr)
   , m_coverageBuilder(std::make_unique<CoverageBuilder>())
   , m_strategyManager(std::make_unique<StrategyManager>())
{
   //auto stat = WFM::GetSharedInstance<Statistic>(DBG_DATA);
   /*connect(this, &PathFinder::SubTaskDataUpdate
         , stat.get(), &Statistic::StatUpdateSubTaskRun
         , Qt::QueuedConnection);
   connect(this, &PathFinder::TaskDataUpdate
         , stat.get(), &Statistic::StatUpdateTaskRun
         , Qt::QueuedConnection);*/
}

PathFinder::~PathFinder()
{}

// TODO: выяснить, что за зверь такой
/*void fly(Route& route)
{}*/

// WARNING: распараллелено!!!
void PathFinder::FindPath(strategy_settings settings, std::shared_ptr<route_data> routeData, const std::shared_ptr<Matrix<SVCG::route_point>> rawdata, const path_finder_settings pathFinderSettings, path_finder_statistic& statistic)
{
   //m_statistic = &statistic;
   //m_vmeta = ExperimentMeta{
   //   pathFinderSettings.statFieldIndex,
   //   pathFinderSettings.multithread
   //};
   //m_rawdata = rawdata;
   //qint64 startTime = CURTIME_MS();
   m_rowCount = rawdata->GetRowCount();
   m_colCount = rawdata->GetColCount();
   size_t iterations = 2;
   std::vector<route> resultAirRoutes;
   resultAirRoutes.resize(routeData->air_routes.size());
   m_funLandVector.clear();
   m_appSettings = std::make_shared<settings::application_settings>();//WFM::GetSharedInstance<Dispatcher>(DBG_DATA)->GetSettings();
   bool pathFound = false;
   if (/*pathFinderSettings.multithread*/false)
   {      
      do
      {
         // test 4/8
         const int threadCount = 16;   // NOTE: По количеству логических ядер 8+HT
         //QThreadPool::globalInstance()->setMaxThreadCount(threadCount);
         //m_funAirVector.clear();
         resultAirRoutes.clear();
         resultAirRoutes.resize(routeData->air_routes.size());
         if (!pathFinderSettings.research)
            m_strategyManager->PrepareControlPoint(settings, iterations, routeData->land_routes, resultAirRoutes, rawdata);
         else
            resultAirRoutes = routeData->air_routes;
         bool finished = true;
         size_t idxDelta = 0;
         while (!finished)
         {
            m_synchronizer.clear();
            for (size_t idx = 0; idx < pathFinderSettings.packet_size == 0 ? resultAirRoutes.size() : std::min(pathFinderSettings.packet_size, resultAirRoutes.size() - idxDelta - pathFinderSettings.packet_size); idx++)
            {
               //m_synchronizer.emplace_back(std::async(std::launch::async, &PathFinder::findAirPath, resultAirRoutes.at(idxDelta + idx), iterations, pathFinderSettings.multithread));
               //qDebug() << "added task" << idx;
            }

            if (pathFinderSettings.packet_size == 0 || idxDelta >= resultAirRoutes.size())
               finished = true;
            //m_funAirVector.emplace_back(run(this, &PathFinder::findAirPath, resultAirRoutes.at(idx), iterations, multithread));
            //m_synchronizer.waitForFinished();
            //qDebug() << "task list finished";
            for (size_t idx = 0; idx < m_synchronizer.size(); idx++)
            {
               resultAirRoutes.at(idxDelta + idx) = m_synchronizer.at(idx).get();
            }
            idxDelta += pathFinderSettings.packet_size;
         }

         if (pathFinderSettings.land_path)
         {
            auto matrix = m_coverageBuilder->BuildLandCoverage(m_rowCount, m_colCount, settings, resultAirRoutes);

            m_funLandVector.clear();
            for (size_t idx = 0; idx < routeData.get()->land_routes.size(); idx++)
            {
               //m_funLandVector.emplace_back(std::async(std::launch::async, &PathFinder::findLandPath, routeData.get()->land_routes.at(idx), matrix, pathFinderSettings.multithread, &pathFound));
               pathFound = true;
            }
            for (size_t idx = 0; idx < m_funLandVector.size(); idx++)
            {
               //m_funLandVector.at(idx).waitForFinished();
               routeData.get()->land_routes.at(idx) = m_funLandVector.at(idx).get();
            }
         }
         else
            pathFound = true;

         //qDebug() << pathFound << iterations;
         iterations++;
      }
      while(!pathFound);
   }
   else
   {      
      do
      {
         resultAirRoutes.clear();
         resultAirRoutes.resize(routeData->air_routes.size());
         if (!pathFinderSettings.research)
            m_strategyManager->PrepareControlPoint(settings, iterations, routeData->land_routes, resultAirRoutes, rawdata);
         else
            resultAirRoutes = routeData->air_routes;
         for (size_t idx = 0; idx < resultAirRoutes.size(); idx++)
            resultAirRoutes.at(idx) = findAirPath(resultAirRoutes.at(idx), rawdata, iterations, pathFinderSettings.multithread);

         if (pathFinderSettings.land_path)
         {
            auto matrix = m_coverageBuilder->BuildLandCoverage(m_rowCount, m_colCount, settings, resultAirRoutes);

            for (size_t idx = 0; idx < routeData.get()->land_routes.size(); idx++)
               routeData.get()->land_routes.at(idx) = findLandPath(routeData.get()->land_routes.at(idx), rawdata, matrix, pathFinderSettings.multithread, &pathFound);
         }
         else
            pathFound = true;

         //qDebug() << pathFound << iterations;
         iterations++;
      }
      while (!pathFound);
   }
   routeData->air_routes = std::move(resultAirRoutes);
   //qint64 finishTime = CURTIME_MS();
   //qint64 time = finishTime - startTime;
   //qDebug() << "path finding ended: mt: " << pathFinderSettings.multithread << " :" << time;
   //TaskStat stat{m_vmeta, time};
   //QVariant vdata;
   //vdata.setValue(stat);
   //WFM::GetSharedInstance<Statistic>(DBG_DATA)->StatUpdateTaskRun(vdata);
   /*if (m_appSettings->type == STT::ApplicationRunType::ART_RESEARCH
       && m_appSettings->res_settings.type == STT::ResearchType::RT_GEN1)
      emit TaskDataUpdate(vdata);*/
   //clearSupportData();
   statistic = *m_statistic;
}

route PathFinder::findAirPath(route& route, const std::shared_ptr<Matrix<SVCG::route_point>> rawdata, size_t iterations, bool multithread)
{
   std::vector<SVCG::route_point> exp_route;
   std::vector<SVCG::route_point> waypointList;
   waypointList.emplace_back(route.start);
   waypointList.insert(waypointList.end(), route.control_point_list.begin(), route.control_point_list.end());
   waypointList.emplace_back(route.finish);
   //Q_ASSERT(waypointList.size() >= 2);
   //Q_UNUSED(iterations);
   // NOTE: Миграция чего-то непонятно чего...
   for (auto& item : waypointList)   
      item = rawdata->Get(item.row, item.col);
   
   aff_checker checker = [](const std::shared_ptr<Matrix<SVCG::route_point>>& data, std::shared_ptr<Matrix<size_t>>& coverageMatrix, size_t row, size_t col) -> bool
   {
      //Q_UNUSED(coverageMatrix);
      return data->Get(row, col).fly == FlyZoneAffilation::FZA_FORBIDDEN;
   };
   height_corrector corrector = [](float y) -> float
   {
      return y + 50.f;
   };
   path_finder_logic logic = { checker, corrector };
   std::string wplist;
   for (auto wp : waypointList)
      wplist.append(std::string("[" + std::to_string(wp.row) + ";" + std::to_string(wp.col) + "]").c_str());
   /*THREADDEBUG("st-fn: "
            << std::string("[" + std::to_string(route.start.mp.row) + ";" + std::to_string(route.start.mp.col) + "]").c_str()
            << std::string("[" + std::to_string(route.finish.mp.row) + ";" + std::to_string(route.finish.mp.col) + "]").c_str());
   THREADDEBUG("wp list: " << waypointList.size() << " elems: " << wplist.c_str());*/
   exp_route.emplace_back(waypointList.at(0));
   for (size_t idx = 0; idx < waypointList.size() - 1; idx++)
   {
      bool found = false;
      auto matrix = std::make_shared<Matrix<size_t>>(m_rowCount, m_colCount, 0);
      std::vector<SVCG::route_point> path = findPath(rawdata, waypointList.at(idx), waypointList.at(idx + 1), logic, matrix, multithread, &found);
      //if (!found)
         //Q_ASSERT(false);
      //exp_route.clear();
      std::reverse(path.begin(), path.end());
      //exp_route.emplace_back(waypointList.at(idx));
      exp_route.back().height += 50.f;
      if (path.size() != 0)
      {
         exp_route.insert(exp_route.end(), ++path.begin(), path.end());
         //exp_route.emplace_back(waypointList.at(idx + 1));
         exp_route.back().height += 50.f;
      }
   }
   //THREADDEBUG("exproute size air: " << exp_route.size());
   route.route_list = exp_route;
   return route;
}

route PathFinder::findLandPath(route& route, const std::shared_ptr<Matrix<SVCG::route_point>> rawdata, std::shared_ptr<Matrix<size_t>> coverageMatrix, bool multithread, bool* pathFounded)
{
   std::vector<SVCG::route_point> exp_route;
   std::vector<SVCG::route_point> waypointList;
   waypointList.emplace_back(route.start);
   waypointList.insert(waypointList.end(), route.control_point_list.begin(), route.control_point_list.end());
   waypointList.emplace_back(route.finish);
   ATLASSERT(waypointList.size() >= 2);
   aff_checker checker = [](const std::shared_ptr<Matrix<SVCG::route_point>>& data, std::shared_ptr<Matrix<size_t>>& covMatrix, size_t row, size_t col) -> bool
   {
      return data->Get(row, col).go == GoZoneAffilation::GZA_FORBIDDEN || covMatrix->Get(row, col) == 0;
   };
   height_corrector counter = [](float y) -> float
   {
      // NOTE: корректировок нет
      return y;
   };
   path_finder_logic logic = { checker, counter };
   exp_route.emplace_back(waypointList.at(0));
   for (size_t idx = 0; idx < waypointList.size() - 1; idx++)
   {
      std::vector<SVCG::route_point> path = findPath(rawdata, waypointList.at(idx), waypointList.at(idx + 1), logic, coverageMatrix, multithread, pathFounded);
      //exp_route.clear();
      //qDebug() << "wpl sz:" << idx;
      std::reverse(path.begin(), path.end());
      //exp_route.emplace_back(waypointList.at(idx));
      if (path.size() != 0)
         exp_route.insert(exp_route.end(), ++path.begin(), path.end());
      //exp_route.emplace_back(waypointList.at(idx + 1));
   }
   //THREADDEBUG("exproute size land: " << exp_route.size());
   route.route_list = exp_route;
   return route;
}

std::vector<SVCG::route_point> PathFinder::findPath(const std::shared_ptr<Matrix<SVCG::route_point>> rawdata, SVCG::route_point& start, SVCG::route_point& finish, path_finder_logic& logic, std::shared_ptr<Matrix<size_t>> coverageMatrix, bool multithread, bool* pathFound)
{
   std::vector<SVCG::route_point> exp_route;
   /*auto countDist = [](RoutePoint& p1, RoutePoint& p2)->double
   {
       return sqrt(pow(p2.x - p1.x, 2.) + pow(p2.z - p1.z, 2.));
   };*/
   //double maxDist = countDist(start.rp, finish.rp);

   if (start.row == finish.row
       && start.col == finish.col)
   {
      auto point = SVCG::route_point(start.row,
                                       start.col,
                                       logic.corrector(start.height),
                                       rawdata->Get(start.row, start.col).fly,
                                       rawdata->Get(start.row, start.col).go);
      exp_route.emplace_back(point);
      *pathFound = true;
      return exp_route;
   }

   auto isInRowSafeRange = [this](int rowIdx)->bool
   {
      return rowIdx >= 0 && rowIdx < static_cast<int>(m_rowCount);
   };
   auto isInColSafeRange = [this](int colIdx)->bool
   {
      return colIdx >= 0 && colIdx < static_cast<int>(m_colCount);
   };

   //qint64 beginFullTime = CURTIME_MS();
   //qint64 beginCreateMatrixTime = CURTIME_MS();
   // NOTE: волновая матрица
   auto pointScore = std::make_shared<Matrix<size_t>>(m_rowCount, m_colCount, 0);
   ////XFM::CreateMatrix(m_pointScore, m_rowCount, m_colCount);
   ////TP_MATRIX_CREATE(pointScore, m_rowCount, m_colCount, size_t);
   //qint64 endCreateMatrixTime = CURTIME_MS();

   std::vector<std::pair<size_t, size_t>> frontline = { { start.row, start.col } };
   size_t stCurRow = frontline.at(0).first;
   size_t stCurCol = frontline.at(0).second;

   for (size_t ridx = 0; ridx < pointScore->GetRowCount(); ridx++)
      for (size_t cidx = 0; cidx < pointScore->GetColCount(); cidx++)
         pointScore->Set(ridx, cidx, logic.checker(rawdata, coverageMatrix, ridx, cidx) ? /*ULONG_LONG_MAX*/ULLONG_MAX : 0);
   pointScore->Set(stCurRow, stCurCol, 1);
   //m_pointScore[stCurRow][stCurCol] = 1;
   bool find = false;
   bool traverse = true;
   size_t frow = finish.row, fcol = finish.col;
   size_t pointScoreCount = 0;

   //qint64 beginMatrixBuildTime = CURTIME_MS();
   // NOTE: построение фронта волны
   while(!find && traverse)
   {
      traverse = false;
      std::vector<std::pair<size_t, size_t>> newFrontline;
      for (auto idx = 0; idx < frontline.size(); idx++)
      {
         size_t curRow = frontline.at(idx).first;
         size_t curCol = frontline.at(idx).second;
         size_t score =/* m_pointScore[curRow][curCol];*/pointScore->Get(curRow, curCol);
         //qDebug() << "Score: " << score << " size " << frontline.size() << " row: " << curRow << " col: " << curCol;
         std::vector<std::pair<size_t, size_t>> pts;
         if (isInRowSafeRange(static_cast<int>(curRow) - 1))
            pts.push_back({curRow - 1, curCol});
         if (isInColSafeRange(static_cast<int>(curCol) - 1))
            pts.push_back({curRow, curCol - 1});
         if (isInRowSafeRange(static_cast<int>(curRow) + 1))
            pts.push_back({curRow + 1, curCol});
         if (isInColSafeRange(static_cast<int>(curCol) + 1))
            pts.push_back({curRow, curCol + 1});
         for (auto iter : pts)
         {            
            if (pointScore->Get(iter.first, iter.second) == 0)
            //if (m_pointScore[iter.first][iter.second] == 0)
            {
               //pointScoreCount++;
               if (logic.checker(rawdata, coverageMatrix, iter.first, iter.second))
               {
                  pointScore->Set(iter.first, iter.second, /*ULONG_LONG_MAX*/ULLONG_MAX);
                  //m_pointScore[iter.first][iter.second] = ULONG_LONG_MAX;
                  pointScoreCount++;
               }
               else
               {
                  newFrontline.push_back({iter.first, iter.second});
                  pointScore->Set(iter.first, iter.second, score + 1);
                  //m_pointScore[iter.first][iter.second] = score + 1;
                  pointScoreCount++;
                  traverse = true;
                  if (frow == iter.first && fcol == iter.second)
                  {
                     find = true;
                     break;
                  }
               }               
            }
         }
      }
      frontline = newFrontline;
   }
   //qint64 endMatrixBuildTime = CURTIME_MS();
   //qDebug() << pointScoreCount;

   *pathFound = find;
   // NOTE: вариант, когда матрица не добралась до финиша - пути точно нет
   if (!find)
      return exp_route;
   // ===

   find = false;
   std::pair<size_t, size_t> curPoint = { finish.row, finish.col };
   size_t curScore = pointScore->Get(finish.row, finish.col);
   //size_t curScore = m_pointScore[finish.mp.row][finish.mp.col];
   size_t curRow = curPoint.first;
   size_t curCol = curPoint.second;
   size_t stRow = start.row;
   size_t stCol = start.col;
   size_t pathRecoverCount = 0;
   auto dist = [](size_t row1, size_t col1, size_t row2, size_t col2)->double
   {
      return sqrt(pow(static_cast<double>(row2) - static_cast<double>(row1), 2.) + pow(static_cast<double>(col2) - static_cast<double>(col1), 2.));
   };
   //qint64 beginBackfireTime = CURTIME_MS();
   // NOTE: обратка
   while(!find)
   {
      find = true;
      if (stRow == curRow && stCol == curCol)
         break;
      auto rp = rawdata->Get(curRow, curCol);
      SVCG::route_point current = { rp.row,
                                    rp.col,
                                    logic.corrector(rp.height),
                                    rp.fly,
                                    rp.go };
      //qDebug() << "Route point: " << current.mp.col << current.mp.row;
      exp_route.emplace_back(current);
      std::vector<std::pair<double, std::pair<size_t, size_t>>> pts;
      if (isInRowSafeRange(curRow))
         pts.push_back({dist(stRow, stCol, curRow - 1, curCol), {curRow - 1, curCol}});
      if (isInColSafeRange(curCol))
         pts.push_back({dist(stRow, stCol, curRow, curCol - 1), {curRow, curCol - 1}});
      if (isInRowSafeRange(curRow))
         pts.push_back({dist(stRow, stCol, curRow + 1, curCol), {curRow + 1, curCol}});
      if (isInColSafeRange(curCol))
         pts.push_back({dist(stRow, stCol, curRow, curCol + 1), {curRow, curCol + 1}});
      std::sort(pts.begin(), pts.end(), [](std::pair<double, std::pair<size_t, size_t>> r1, std::pair<double, std::pair<size_t, size_t>> r2)->bool { return r1.first < r2.first; });
      //std::sort(pts.begin(), pts.end(), [](QPair<double, QPair<size_t, size_t>> r1, QPair<double, QPair<size_t, size_t>> r2)->bool { return r1.first > r2.first; });

      for (auto iter : pts)
      {
         if (pointScore->Get(iter.second.first, iter.second.second) != 0
             && pointScore->Get(iter.second.first, iter.second.second) < curScore)
         /*if (m_pointScore[iter.second.first][iter.second.second] != 0
             && m_pointScore[iter.second.first][iter.second.second] < curScore)*/
         {
            curRow = iter.second.first;
            curCol = iter.second.second;
            curScore = pointScore->Get(curRow, curCol);
            //curScore = m_pointScore[curRow][curCol];
            //qDebug() << "cs: " << curScore;
            find = false;
            break;
         }
      }
   }
   auto rp = rawdata->Get(curRow, curCol);
   exp_route.emplace_back(SVCG::route_point{ rp.row,
                                             rp.col,
                                             logic.corrector(rp.height),
                                             rp.fly,
                                             rp.go });
   //qint64 endBackfireTime = CURTIME_MS();
   //qint64 endFullTime = CURTIME_MS();
   //qint64 fullTime = endFullTime - beginFullTime;
   //qint64 crTime = endCreateMatrixTime - beginCreateMatrixTime;
   //qint64 bldTime = endMatrixBuildTime - beginMatrixBuildTime;
   //qint64 bfTime = endBackfireTime - beginBackfireTime;
   /*qDebug() << "full:" << fullTime;
   qDebug() << "cr:" << crTime;
   qDebug() << "bld:" << bldTime;
   qDebug() << "backfire:" << bfTime;*/
   //Q_ASSERT();
   //TP_MATRIX_CLEAR(pointScore, m_rowCount);
   //SubTaskStat data{m_vmeta, fullTime, crTime, bldTime, bfTime, pointScoreCount};
   //QVariant vdata;
   //vdata.setValue(data);
   //qDebug() << "fp run";
   //WFM::GetSharedInstance<Statistic>(DBG_DATA)->StatUpdateSubTaskRun(vdata);
   /*if (m_appSettings->type == STT::ApplicationRunType::ART_RESEARCH
       && m_appSettings->res_settings.type == STT::ResearchType::RT_GEN1)
      emit SubTaskDataUpdate(vdata);*/
   return exp_route;
}
