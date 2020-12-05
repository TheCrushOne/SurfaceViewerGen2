#include "stdafx.h"
#include "PathFinder.h"
#include <math.h>
#include <limits.h>
#include <future>
#include <algorithm>
//#include "framework/base/instance.h"
//#include "common/matrix.h"
//#include "main/engine.h"
//#include "main/dispatcher.h"
//#include "settings/common/settings.h"

#define THREADDEBUG(line) qDebug() << line << "thread:" << QThread::currentThreadId();

using namespace SV;
using namespace SV::pathfinder;

PathFinder::PathFinder()
{}

PathFinder::~PathFinder()
{}

// TODO: выяснить, что за зверь такой
/*void fly(Route& route)
{}*/

namespace
{
   bool airCheckAffilation(const SharedRoutePointMatrix& data, const SharedUnsignedMatrix& coverageMatrix, size_t row, size_t col)
   {
      return data->Get(row, col).fly == FlyZoneAffilation::FZA_FORBIDDEN;
   }

   float airCorrector(float y)
   {
      return y + 50.f;
   }
}

void PathFinder::FindAirPath(settings::route& route, const SharedRoutePointMatrix& rawdata, size_t iterations, bool multithread)
{
   CG::route_line exp_route;
   CG::route_line waypointList;
   waypointList.emplace_back(route.start);
   waypointList.insert(waypointList.end(), route.control_point_list.begin(), route.control_point_list.end());
   waypointList.emplace_back(route.finish);
   //Q_ASSERT(waypointList.size() >= 2);
   //Q_UNUSED(iterations);
   // NOTE: Миграция чего-то непонятно чего...
   for (auto& item : waypointList)
      item = rawdata->Get(item.row, item.col);

   path_finder_logic logic = { airCheckAffilation, airCorrector };
   std::string wplist;
   for (const auto& wp : waypointList)
      wplist.append(std::string("[" + std::to_string(wp.row) + ";" + std::to_string(wp.col) + "]").c_str());
   /*THREADDEBUG("st-fn: "
            << std::string("[" + std::to_string(route.start.mp.row) + ";" + std::to_string(route.start.mp.col) + "]").c_str()
            << std::string("[" + std::to_string(route.finish.mp.row) + ";" + std::to_string(route.finish.mp.col) + "]").c_str());
   THREADDEBUG("wp list: " << waypointList.size() << " elems: " << wplist.c_str());*/
   exp_route.emplace_back(waypointList.at(0));
   for (size_t idx = 0; idx < waypointList.size() - 1; idx++)
   {
      bool found = false;
      auto matrix = std::make_shared<UnsignedMatrix>(rawdata->GetRowCount(), rawdata->GetColCount(), 0);
      CG::route_line path = findUniversalPath(waypointList.at(idx), waypointList.at(idx + 1), logic, rawdata, matrix, multithread, &found);
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
}

namespace
{
   bool landAffilationChecker(const SharedRoutePointMatrix& data, const SharedUnsignedMatrix& covMatrix, size_t row, size_t col)
   {
      return data->Get(row, col).go == GoZoneAffilation::GZA_FORBIDDEN || covMatrix->Get(row, col) == 0;
   }

   float landCorrector(float y)
   {
      // NOTE: корректировок нет
      return y;
   }
}

void PathFinder::FindLandPath(settings::route& route, const SharedRoutePointMatrix& rawdata, const SharedUnsignedMatrix& coverageMatrix, bool multithread, bool *pathfound)
{
   CG::route_line exp_route;
   CG::route_line waypointList;
   waypointList.emplace_back(route.start);
   waypointList.insert(waypointList.end(), route.control_point_list.begin(), route.control_point_list.end());
   waypointList.emplace_back(route.finish);
   ATLASSERT(waypointList.size() >= 2);

   path_finder_logic logic = { landAffilationChecker, landCorrector };
   exp_route.emplace_back(waypointList.at(0));
   for (size_t idx = 0; idx < waypointList.size() - 1; idx++)
   {
      CG::route_line path = findUniversalPath(waypointList.at(idx), waypointList.at(idx + 1), logic, rawdata, coverageMatrix, multithread, pathfound);
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
}

CG::route_line PathFinder::findUniversalPath(const CG::route_point& start, const CG::route_point& finish, const path_finder_logic& logic, const SharedRoutePointMatrix& rawdata, const SharedUnsignedMatrix& coverageMatrix, bool multithread, bool* pathFound)
{
   CG::route_line exp_route;
   /*auto countDist = [](RoutePoint& p1, RoutePoint& p2)->double
   {
       return sqrt(pow(p2.x - p1.x, 2.) + pow(p2.z - p1.z, 2.));
   };*/
   //double maxDist = countDist(start.rp, finish.rp);

   if (start.row == finish.row
       && start.col == finish.col)
   {
      auto point = CG::route_point(
         start.row,
         start.col,
         logic.corrector(start.height),
         rawdata->Get(start.row, start.col).fly,
         rawdata->Get(start.row, start.col).go
      );
      exp_route.emplace_back(point);
      *pathFound = true;
      return exp_route;
   }

   auto isInRowSafeRange = [rawdata, this](int rowIdx)->bool
   {
      return rowIdx >= 0 && rowIdx < static_cast<int>(rawdata->GetRowCount());
   };
   auto isInColSafeRange = [rawdata, this](int colIdx)->bool
   {
      return colIdx >= 0 && colIdx < static_cast<int>(rawdata->GetColCount());
   };

   //qint64 beginFullTime = CURTIME_MS();
   //qint64 beginCreateMatrixTime = CURTIME_MS();
   // NOTE: волновая матрица
   auto pointScore = std::make_shared<UnsignedMatrix>(rawdata->GetRowCount(), rawdata->GetColCount(), 0);
   ////XFM::CreateMatrix(m_pointScore, m_rowCount, m_colCount);
   ////TP_MATRIX_CREATE(pointScore, m_rowCount, m_colCount, size_t);
   //qint64 endCreateMatrixTime = CURTIME_MS();

   std::vector<std::pair<size_t, size_t>> frontline = { { start.row, start.col } };
   size_t stCurRow = frontline.at(0).first;
   size_t stCurCol = frontline.at(0).second;

   for (size_t ridx = 0; ridx < pointScore->GetRowCount(); ridx++)
      for (size_t cidx = 0; cidx < pointScore->GetColCount(); cidx++)
         pointScore->Set(ridx, cidx, logic.GetChecker()(rawdata, coverageMatrix, ridx, cidx) ? ULLONG_MAX : 0);
   pointScore->Set(stCurRow, stCurCol, 1);
   //m_pointScore[stCurRow][stCurCol] = 1;
   bool find = false;
   bool traverse = true;
   size_t frow = finish.row, fcol = finish.col;
   size_t pointScoreCount = 0;

   auto frontlineCheckEmplace = [isInRowSafeRange, isInColSafeRange](std::vector<std::pair<size_t, size_t>>& storage, size_t row, size_t col)
   {
      if (isInRowSafeRange(static_cast<int>(row)) && isInColSafeRange(static_cast<int>(col)))
         storage.emplace_back(std::make_pair(row, col));
   };
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
         size_t score = pointScore->Get(curRow, curCol);
         //qDebug() << "Score: " << score << " size " << frontline.size() << " row: " << curRow << " col: " << curCol;
         std::vector<std::pair<size_t, size_t>> pts;
         // NOTE: если понадобится вернуть 4 направления - срезать лишнее только тут
         frontlineCheckEmplace(pts, curRow - 1, curCol - 1);   // topleft
         frontlineCheckEmplace(pts, curRow - 1, curCol);       // top
         frontlineCheckEmplace(pts, curRow - 1, curCol + 1);   // topright
         frontlineCheckEmplace(pts, curRow, curCol - 1);       // left
         frontlineCheckEmplace(pts, curRow, curCol + 1);       // right
         frontlineCheckEmplace(pts, curRow + 1, curCol - 1);   // bottomleft
         frontlineCheckEmplace(pts, curRow + 1, curCol);       // bottom
         frontlineCheckEmplace(pts, curRow + 1, curCol + 1);   // bottomright

         for (auto iter : pts)
         {            
            if (pointScore->Get(iter.first, iter.second) == 0)
            {
               if (logic.checker(rawdata, coverageMatrix, iter.first, iter.second))
               {
                  pointScore->Set(iter.first, iter.second, ULLONG_MAX);
                  pointScoreCount++;
               }
               else
               {
                  newFrontline.push_back({iter.first, iter.second});
                  pointScore->Set(iter.first, iter.second, score + 1);
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
   double fnRowD = static_cast<double>(finish.row);
   double fnColD = static_cast<double>(finish.col);
   double stRowD = static_cast<double>(start.row);
   double stColD = static_cast<double>(start.col);
   size_t pathRecoverCount = 0;
   auto distToLine = [stRowD, stColD, fnRowD, fnColD](size_t row, size_t col)->double
   {
      double fRow = static_cast<double>(row);
      double fCol = static_cast<double>(col);
      return (fabs((fnColD - stColD)*fRow - (fnRowD - stRowD)*fCol + fnRowD*stColD - fnColD*stRowD))/sqrt(pow(fnColD - stColD, 2.) + pow(fnRowD - stRowD, 2.));
   };
   auto expRouteCheckEmplace = [isInRowSafeRange, isInColSafeRange, distToLine](std::vector<std::pair<double, std::pair<size_t, size_t>>>& pts, size_t curRow, size_t curCol)
   {
      if (isInRowSafeRange(curRow) && isInColSafeRange(curCol))
         pts.emplace_back(std::make_pair(distToLine(curRow, curCol), std::make_pair(curRow, curCol)));
   };
   //qint64 beginBackfireTime = CURTIME_MS();
   // NOTE: обратка
   while(!find)
   {
      find = true;
      if (stRow == curRow && stCol == curCol)
         break;
      auto rp = rawdata->Get(curRow, curCol);
      CG::route_point current = { 
         rp.row,
         rp.col,
         logic.corrector(rp.height),
         rp.fly,
         rp.go
      };
      //qDebug() << "Route point: " << current.mp.col << current.mp.row;
      exp_route.emplace_back(current);
      std::vector<std::pair<double, std::pair<size_t, size_t>>> pts;

      expRouteCheckEmplace(pts, curRow - 1, curCol - 1);  // topleft
      expRouteCheckEmplace(pts, curRow - 1, curCol);      // top
      expRouteCheckEmplace(pts, curRow - 1, curCol + 1);  // topright
      expRouteCheckEmplace(pts, curRow, curCol - 1);      // left
      expRouteCheckEmplace(pts, curRow, curCol - 1);      // right
      expRouteCheckEmplace(pts, curRow + 1, curCol - 1);  // bottomleft
      expRouteCheckEmplace(pts, curRow + 1, curCol);      // bottom
      expRouteCheckEmplace(pts, curRow + 1, curCol + 1);  // bottomright

      std::sort(pts.begin(), pts.end(), [](std::pair<double, std::pair<size_t, size_t>> r1, std::pair<double, std::pair<size_t, size_t>> r2)->bool { return r1.first < r2.first; });

      for (const auto& iter : pts)
      {
         if (pointScore->Get(iter.second.first, iter.second.second) != 0
             && pointScore->Get(iter.second.first, iter.second.second) < curScore)
         {
            curRow = iter.second.first;
            curCol = iter.second.second;
            curScore = pointScore->Get(curRow, curCol);
            //qDebug() << "cs: " << curScore;
            find = false;
            break;
         }
      }
   }
   auto rp = rawdata->Get(curRow, curCol);
   exp_route.emplace_back(
      CG::route_point{
         rp.row,
         rp.col,
         logic.corrector(rp.height),
         rp.fly,
         rp.go
      }
   );
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
