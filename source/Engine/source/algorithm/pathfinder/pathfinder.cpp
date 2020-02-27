#include "stdafx.h"
#include "pathfinder.h"
#include <math.h>
#include <limits.h>
//#include "framework/base/instance.h"
//#include "common/matrix.h"
#include "main/engine.h"
//#include "main/dispatcher.h"
//#include "settings/common/settings.h"


#define FLYCRCHCKPOINT(row, col, data) CombinedPoint{ RoutePoint(0.f, 0.f, 0.f, FZA_NORMAL, GZA_NORMAL), \
                        checkRetranslateFlyPointAffilation(static_cast<int>(row), static_cast<int>(col), data) }

#define DMPCORR(oldrow, oldcol, nwp) if (nwp.mp.row != oldrow || nwp.mp.col != oldcol) \
                              qDebug() << "Corr: [ " << oldrow << ", " << oldcol << " ] -> [ " << nwp.mp.row << ", " << nwp.mp.col << " ]";

#define THREADDEBUG(line) qDebug() << line << "thread:" << QThread::currentThreadId();

using namespace pathfinder;

PathFinder::PathFinder()
   //: m_data(LightPointData())
   //: m_pointScore(0)
   : m_rowCount(0)
   , m_colCount(0)
   , m_statistic(nullptr)
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

//std::shared_ptr<SVM::iMatrix<size_t>> PathFinder::buildLandCoverage(StrategySettings settings, std::vector<Route>& airRoutes)
//{
    //auto countDist = [](int x1, int y1, int x2, int y2)->double
    //{
    //    return sqrt(pow(y2 - y1, 2.) + pow(x2 - x1, 2.));
    //};
    //auto coverageMatrix = WFM::CreateSharedObject<SVM::iMatrix<size_t>>(m_rowCount, m_colCount, 0);
    //int radius = static_cast<int>(settings.radius) + 2;   // NOTE: костыль

    //for (size_t airIdx = 0; airIdx < airRoutes.size(); airIdx++)
    //{
    //   for (auto cp : airRoutes.at(airIdx).route)
    //   {
    //      auto mp = cp.mp;
    //      //bool pointInRadius = true;
    //      //double idxRingStep = 1;
    //      int lowerRowPoint = (static_cast<int>(mp.row) - radius >= 0 ? static_cast<int>(mp.row) - radius : 0);
    //      int higherRowPoint = (static_cast<int>(mp.row) + radius < static_cast<int>(m_rowCount) ? static_cast<int>(mp.row) + radius : static_cast<int>(m_rowCount) - 1);
    //      int lowerColPoint = (static_cast<int>(mp.col) - radius >= 0 ? static_cast<int>(mp.col) - radius : 0);
    //      int higherColPoint = (static_cast<int>(mp.col) + radius < static_cast<int>(m_colCount) ? static_cast<int>(mp.col) + radius : static_cast<int>(m_colCount) - 1);
    //      for (int row = lowerRowPoint; row <= higherRowPoint; row++)
    //      {
    //         for (int col = lowerColPoint; col <= higherColPoint; col++)
    //         {
    //            if (countDist(row, col, static_cast<int>(mp.row), static_cast<int>(mp.col)) < radius)
    //               coverageMatrix->Set(static_cast<size_t>(row), static_cast<size_t>(col), 1);
    //         }
    //      }
    //   }
    //}

    //QString time_format = "ddMMyyyy_HHmm";
    //QDateTime a = QDateTime::currentDateTime();
    //QString as = a.toString(time_format);
    //auto stt = WFM::GetSharedInstance<Dispatcher>(DBG_DATA)->GetSettings();
    //QString nwDir;// = stt->sim_settings.fileinfo.path + "/../export/" + as + "/";
    //bool exist = false;
    //size_t idx = 0;
    //do
    //{
    //   exist = false;
    //   nwDir = stt->sim_settings.fileinfo.path + "/../export/" + as + "_" + QString::number(idx) + "/";

    //   QDir dir(nwDir);
    //   if (!dir.exists())
    //      dir.mkpath(".");
    //   else
    //      exist = true;
    //   idx++;
    //} while(exist);

    //QString filename = nwDir + "matrix.mtxdmp";
    //QFile file(filename);
    //if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    //{
    //   size_t colCount = coverageMatrix->GetColCount();
    //   size_t rowCount = coverageMatrix->GetRowCount();
    //   QTextStream stream(&file);
    //   for (size_t rowIdx = 0; rowIdx < rowCount; rowIdx++)
    //   {
    //      for (size_t colIdx = 0; colIdx < colCount; colIdx++)
    //         stream << coverageMatrix->Get(rowIdx, colIdx) << " ";
    //      stream << "\n";
    //   }
    //   file.close();
    //}

   //auto coverageMatrix = std::make_shared<SVM::iMatrix<size_t>>(0, 0, 0) ; //WFM::CreateSharedObject<SVM::iMatrix<size_t>>(m_rowCount, m_colCount, 0);
   //return coverageMatrix;
//}

bool PathFinder::landPathCoverage(strategy_settings settings, std::vector<route>& landRoutes, std::vector<route>& airRoutes)
{
   //auto countDist = [](int x1, int y1, int x2, int y2)->double
   //{
   //    return sqrt(pow(y2 - y1, 2.) + pow(x2 - x1, 2.));
   //};
   //auto coverageMatrix = WFM::CreateSharedObject<SVM::iMatrix<size_t>>(m_rowCount, m_colCount, 0);
   //int radius = static_cast<int>(settings.radius) + 2;   // NOTE: костыль

   //for (size_t airIdx = 0; airIdx < airRoutes.size(); airIdx++)
   //{
   //   for (auto cp : airRoutes.at(airIdx).route)
   //   {
   //      auto mp = cp.mp;
   //      //bool pointInRadius = true;
   //      //double idxRingStep = 1;
   //      int lowerRowPoint = (static_cast<int>(mp.row) - radius >= 0 ? static_cast<int>(mp.row) - radius : 0);
   //      int higherRowPoint = (static_cast<int>(mp.row) + radius < static_cast<int>(m_rowCount) ? static_cast<int>(mp.row) + radius : static_cast<int>(m_rowCount) - 1);
   //      int lowerColPoint = (static_cast<int>(mp.col) - radius >= 0 ? static_cast<int>(mp.col) - radius : 0);
   //      int higherColPoint = (static_cast<int>(mp.col) + radius < static_cast<int>(m_colCount) ? static_cast<int>(mp.col) + radius : static_cast<int>(m_colCount) - 1);
   //      for (int row = lowerRowPoint; row <= higherRowPoint; row++)
   //      {
   //         for (int col = lowerColPoint; col <= higherColPoint; col++)
   //         {
   //            if (countDist(row, col, static_cast<int>(mp.row), static_cast<int>(mp.col)) < radius)
   //               coverageMatrix->Set(static_cast<size_t>(row), static_cast<size_t>(col), 1);
   //         }
   //      }
   //   }
   //}

   //QString time_format = "ddMMyyyy_HHmm";
   //QDateTime a = QDateTime::currentDateTime();
   //QString as = a.toString(time_format);
   //auto stt = WFM::GetSharedInstance<Dispatcher>(DBG_DATA)->GetSettings();
   //QString nwDir;
   //bool exist = false;
   //size_t idx = 0;
   //do
   //{
   //   exist = false;
   //   nwDir = stt->sim_settings.fileinfo.path + "/../export/" + as + "_" + QString::number(idx) + "/";

   //   QDir dir(nwDir);
   //   if (!dir.exists())
   //      dir.mkpath(".");
   //   else
   //      exist = true;
   //   idx++;
   //} while(exist);

   //QString filename = nwDir + "matrix.mtxdmp";
   //QFile file(filename);
   //if(file.open(QIODevice::WriteOnly | QIODevice::Text))
   //{
   //   size_t colCount = coverageMatrix->GetColCount();
   //   size_t rowCount = coverageMatrix->GetRowCount();
   //   QTextStream stream(&file);
   //   for (size_t rowIdx = 0; rowIdx < rowCount; rowIdx++)
   //   {
   //      for (size_t colIdx = 0; colIdx < colCount; colIdx++)
   //         stream << coverageMatrix->Get(rowIdx, colIdx) << " ";
   //      stream << "\n";
   //   }
   //   file.close();
   //}

   //for (size_t landIdx = 0; landIdx < landRoutes.size(); landIdx++)
   //{
   //   for (auto cp : landRoutes.at(landIdx).route)
   //   {
   //      auto mp = cp.mp;
   //      if (coverageMatrix->Get(mp.row, mp.col) == 0)
   //         return false;
   //   }
   //}

   return true;
}

SVCG::route_point PathFinder::checkRetranslateFlyPointAffilation(int row, int col/*, const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& rawdata*/)
{   
   //auto rowCount = rawdata->GetRowCount();
   //auto colCount = rawdata->GetColCount();
   //auto checkPointBorderCollision = [&rowCount, &colCount](int row, int col)->bool
   //{
   //    return !(row >= 0 && row < static_cast<int>(rowCount) && col >= 0 && col < static_cast<int>(colCount));
   //};
   //bool /*found = false,*/ stuck = false;
   //int step = 1;
   //if (checkPointBorderCollision(row, col) || rawdata->Get(static_cast<size_t>(row), static_cast<size_t>(col)).fly.fza == FZA_FORBIDDEN)
   //{
   //   while(!stuck)
   //   {
   //      if ((row - step < 0)
   //          && (row + step >= static_cast<int>(rowCount))
   //          && (col - step < 0)
   //          && (col + step >= static_cast<int>(colCount)))
   //      {
   //         stuck = true;
   //         qDebug() << "stucked: " << row << col;
   //         return MatrixPoint(0, 0);
   //      }

   //      if (!checkPointBorderCollision(row - step, col)
   //          && (rawdata->Get(static_cast<size_t>(row - step), static_cast<size_t>(col)).fly.fza != FZA_FORBIDDEN))
   //         return MatrixPoint(static_cast<size_t>(row - step), static_cast<size_t>(col));

   //      if (!checkPointBorderCollision(row + step, col)
   //          && (rawdata->Get(static_cast<size_t>(row + step), static_cast<size_t>(col)).fly.fza != FZA_FORBIDDEN))
   //         return MatrixPoint(static_cast<size_t>(row + step), static_cast<size_t>(col));

   //      if (!checkPointBorderCollision(row, col - step)
   //          && (rawdata->Get(static_cast<size_t>(row), static_cast<size_t>(col - step)).fly.fza != FZA_FORBIDDEN))
   //         return MatrixPoint(static_cast<size_t>(row), static_cast<size_t>(col - step));

   //      if (!checkPointBorderCollision(row, col + step)
   //          && (rawdata->Get(static_cast<size_t>(row), static_cast<size_t>(col + step)).fly.fza != FZA_FORBIDDEN))
   //         return MatrixPoint(static_cast<size_t>(row), static_cast<size_t>(col + step));
   //      step++;
   //      //qDebug() << "corrector step:" << step << row << col;//row << static_cast<int>(rowCount) << (row - step < 0) << (row + step >= static_cast<int>(rowCount)) << (col - step < 0) << (col + step >= static_cast<int>(colCount));
   //   }
   //}
   return SVCG::route_point(static_cast<size_t>(row), static_cast<size_t>(col), 0.f);
}

bool PathFinder::prepareControlPoint(strategy_settings settings, size_t iterations, std::vector<route>& landRoutes, std::vector<route>& airRoutes/*, const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& rawdata*/)
{
   //StrategyType type = settings.type;
   //auto start = landRoutes.at(0).start, finish = landRoutes.at(0).finish;
   ////bool finished = false;
   //int radius = static_cast<int>(settings.radius);
   //int diameter = 2*radius;
   //bool isPlusDirectionRow = start.mp.row < finish.mp.row;
   //bool isPlusDirectionCol = start.mp.col < finish.mp.col;
   //size_t rowCount = rawdata->GetRowCount(), colCount = rawdata->GetColCount();
   //size_t directionControllerPointCount = 20;

   //if (type == ST_SNAKE)
   //{
   //   // NOTE: у этой стратегии не может быть больше одной ходки(полное покрытие)
   //   Q_ASSERT(iterations == 1);
   //   bool fnDirection = true;
   //   int currentRowStLine;
   //   int currentColStLine;
   //   int currentRowFnLine;
   //   int currentColFnLine;
   //   bool first = true;
   //   // NOTE: количество ходок
   //   size_t directionCount = static_cast<size_t>(abs(static_cast<int>(rowCount))/(diameter*static_cast<int>(airRoutes.size()))) + 2;   // WARNING: костыль
   //   size_t droneCount = airRoutes.size();
   //   // NOTE: отступ от границ карты
   //   //int startBorderOffset = radius;

   //   for (size_t airIdx = 0; airIdx < airRoutes.size(); airIdx++)
   //   {
   //      currentRowStLine = static_cast<int>(start.mp.row);
   //      currentColStLine = static_cast<int>(start.mp.col);
   //      currentRowFnLine = static_cast<int>(start.mp.row);
   //      currentColFnLine = static_cast<int>(finish.mp.col);

   //      airRoutes.at(airIdx).start = FLYCRCHCKPOINT(start.mp.row, start.mp.col, rawdata);
   //      //DMPCORR(start.mp.row, start.mp.col, airRoutes.at(airIdx).start);

   //      for (size_t dirIdx = 0; dirIdx < directionCount; dirIdx++)
   //      {
   //         // NOTE: эквивалент(логическая операция)
   //         fnDirection = (dirIdx % 2 == 0) == (isPlusDirectionCol);
   //         if (fnDirection)
   //         {
   //            // TODO: добить
   //            currentRowStLine = static_cast<int>(dirIdx*droneCount)*diameter + static_cast<int>(airIdx)*diameter + radius;// (fnDirection ? 1 : -1)*(isPlusDirectionRow ? 1 : -1)*shift;
   //            currentColStLine = static_cast<int>(droneCount - airIdx - 1)*diameter + radius;
   //            currentRowFnLine = currentRowStLine;
   //            currentColFnLine = static_cast<int>(colCount) - static_cast<int>(airIdx)*diameter - radius;
   //            //currentRowFnLine = (fnDirection ? 1 : -1)*(isPlusDirectionRow ? 1 : -1)*shift;
   //            //if (!first)
   //               //currentColStLine += (fnDirection ? 1 : -1)*(isPlusDirectionCol ? 1 : -1)*shift;
   //            //currentColFnLine += (fnDirection ? 1 : -1)*(isPlusDirectionCol ? -1 : 1)*shift;
   //         }
   //         else
   //         {
   //            currentRowStLine = static_cast<int>(dirIdx*droneCount)*diameter + static_cast<int>(droneCount - airIdx - 1)*diameter + radius;
   //            currentColStLine = static_cast<int>(colCount) - static_cast<int>(airIdx)*diameter - radius;
   //            currentRowFnLine = currentRowStLine;// (fnDirection ? 1 : -1)*(isPlusDirectionRow ? 1 : -1)*shift;
   //            currentColFnLine = static_cast<int>(droneCount - airIdx - 1)*diameter + radius;
   //         }
   //         auto correctedStart = FLYCRCHCKPOINT(currentRowStLine, currentColStLine, rawdata);
   //         auto correctedFinish = FLYCRCHCKPOINT(currentRowFnLine, currentColFnLine, rawdata);
   //         DMPCORR(currentRowStLine, currentColStLine, correctedStart);
   //         DMPCORR(currentRowFnLine, currentColFnLine, correctedFinish);
   //         airRoutes.at(airIdx).controlPointList.emplace_back(correctedStart);

   //         double rowStep = (static_cast<double>(correctedFinish.mp.row) - static_cast<double>(correctedStart.mp.row))/static_cast<double>(directionControllerPointCount);
   //         double colStep = (static_cast<double>(correctedFinish.mp.col) - static_cast<double>(correctedStart.mp.col))/static_cast<double>(directionControllerPointCount);
   //         for (size_t idx = 1; idx < directionControllerPointCount; idx++)
   //         {
   //            auto correctedPoint = FLYCRCHCKPOINT(floor(static_cast<double>(correctedStart.mp.row) + rowStep*idx)
   //                                               , floor(static_cast<double>(correctedStart.mp.col) + colStep*idx)
   //                                               , rawdata);
   //            airRoutes.at(airIdx).controlPointList.emplace_back(correctedPoint);
   //            DMPCORR(floor(static_cast<double>(correctedStart.mp.row) + rowStep*idx)
   //                  , floor(static_cast<double>(correctedStart.mp.col) + colStep*idx)
   //                  , correctedPoint);
   //         }

   //         airRoutes.at(airIdx).controlPointList.emplace_back(correctedFinish);
   //      }

   //      airRoutes.at(airIdx).finish = FLYCRCHCKPOINT(finish.mp.row, finish.mp.col, rawdata);
   //      DMPCORR(finish.mp.row, finish.mp.col, airRoutes.at(airIdx).finish);
   //   }

   //   first = false;
   //}
   //else if (type == ST_SECTOR)
   //{
   //   airRoutes.at(0).start = FLYCRCHCKPOINT(start.mp.row, start.mp.col, rawdata);
   //   double rowStep = (static_cast<double>(finish.mp.row) - static_cast<double>(start.mp.row))/static_cast<double>(directionControllerPointCount);
   //   double colStep = (static_cast<double>(finish.mp.col) - static_cast<double>(start.mp.col))/static_cast<double>(directionControllerPointCount);
   //   for (size_t idx = 1; idx < directionControllerPointCount; idx++)
   //   {
   //      airRoutes.at(0).controlPointList.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(start.mp.row) + rowStep*idx),
   //                                                                         floor(static_cast<double>(start.mp.col) + colStep*idx),
   //                                                                         rawdata));
   //   }
   //   airRoutes.at(0).finish = FLYCRCHCKPOINT(finish.mp.row, finish.mp.col, rawdata);

   //   if (iterations == 1 && airRoutes.size() == 3)
   //      int i = 0;

   //   for (size_t lineIdx = 1; lineIdx < airRoutes.size()*iterations; lineIdx++)
   //   {
   //      size_t airIdx = lineIdx % airRoutes.size();
   //      airRoutes.at(airIdx).start = FLYCRCHCKPOINT(start.mp.row, start.mp.col, rawdata);
   //      int finishRow, finishCol;
   //      if (lineIdx % 2 == 0)
   //      {
   //         finishRow = static_cast<int>(finish.mp.row) + (static_cast<int>(lineIdx) / 2)*(isPlusDirectionRow ? -1 : 1)*diameter;
   //         finishCol = static_cast<int>(finish.mp.col);
   //      }
   //      else
   //      {
   //         finishRow = static_cast<int>(finish.mp.row);
   //         finishCol = static_cast<int>(finish.mp.col) + (static_cast<int>(lineIdx) / 2 + 1)*(isPlusDirectionCol ? -1 : 1)*diameter;
   //      }
   //      auto correctedFinish = FLYCRCHCKPOINT(finishRow, finishCol, rawdata);
   //      double rowStep = (static_cast<double>(correctedFinish.mp.row) - static_cast<double>(start.mp.row))/static_cast<double>(directionControllerPointCount);
   //      double colStep = (static_cast<double>(correctedFinish.mp.col) - static_cast<double>(start.mp.col))/static_cast<double>(directionControllerPointCount);
   //      for (size_t idx = 1; idx < directionControllerPointCount; idx++)
   //      {
   //         airRoutes.at(airIdx).controlPointList.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(start.mp.row) + rowStep*idx),
   //                                                                            floor(static_cast<double>(start.mp.col) + colStep*idx),
   //                                                                            rawdata));
   //      }
   //      airRoutes.at(airIdx).controlPointList.emplace_back(correctedFinish);
   //      airRoutes.at(airIdx).finish = FLYCRCHCKPOINT(finish.mp.row, finish.mp.col, rawdata);
   //   }
   //}
   //else if (type == ST_RHOMBOID)
   //{
   //   //airRoutes.at(0).controlPointList.clear();
   //   airRoutes.at(0).start = FLYCRCHCKPOINT(start.mp.row, start.mp.col, rawdata);
   //   double rowStep = (static_cast<double>(finish.mp.row) - static_cast<double>(start.mp.row))/static_cast<double>(directionControllerPointCount);
   //   double colStep = (static_cast<double>(finish.mp.col) - static_cast<double>(start.mp.col))/static_cast<double>(directionControllerPointCount);
   //   for (size_t idx = 1; idx < directionControllerPointCount; idx++)
   //   {
   //      airRoutes.at(0).controlPointList.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(start.mp.row) + rowStep*idx),
   //                                                                         floor(static_cast<double>(start.mp.col) + colStep*idx),
   //                                                                         rawdata));
   //   }
   //   airRoutes.at(0).finish = FLYCRCHCKPOINT(finish.mp.row, finish.mp.col, rawdata);

   //   for (size_t lineIdx = 1; lineIdx < airRoutes.size()*iterations; lineIdx++)
   //   {
   //      size_t airIdx = lineIdx % airRoutes.size();
   //      //airRoutes.at(airIdx).controlPointList.clear();
   //      airRoutes.at(airIdx).start = FLYCRCHCKPOINT(start.mp.row, start.mp.col, rawdata);
   //      int finishRow, finishCol, startRow, startCol;
   //      if (lineIdx % 2 == 0)
   //      {
   //         startRow = static_cast<int>(start.mp.row) + (static_cast<int>(lineIdx) / 2)*(isPlusDirectionRow ? 1 : -1)*diameter;
   //         startCol = static_cast<int>(start.mp.col);
   //         finishRow = static_cast<int>(finish.mp.row);
   //         finishCol = static_cast<int>(finish.mp.col) - (static_cast<int>(lineIdx) / 2)*(isPlusDirectionCol ? 1 : -1)*diameter;
   //      }
   //      else
   //      {
   //         startRow = static_cast<int>(start.mp.row);
   //         startCol = static_cast<int>(start.mp.col) + (static_cast<int>(lineIdx) / 2 + 1)*(isPlusDirectionCol ? 1 : -1)*diameter;
   //         finishRow = static_cast<int>(finish.mp.row) - (static_cast<int>(lineIdx) / 2 + 1)*(isPlusDirectionRow ? 1 : -1)*diameter;
   //         finishCol = static_cast<int>(finish.mp.col);
   //      }
   //      auto correctedStart = FLYCRCHCKPOINT(startRow, startCol, rawdata);
   //      auto correctedFinish = FLYCRCHCKPOINT(finishRow, finishCol, rawdata);
   //      airRoutes.at(airIdx).controlPointList.emplace_back(correctedStart);
   //      double rowStep = (static_cast<double>(correctedFinish.mp.row) - static_cast<double>(correctedStart.mp.row))/static_cast<double>(directionControllerPointCount);
   //      double colStep = (static_cast<double>(correctedFinish.mp.col) - static_cast<double>(correctedStart.mp.col))/static_cast<double>(directionControllerPointCount);
   //      for (size_t idx = 1; idx < directionControllerPointCount; idx++)
   //      {
   //         airRoutes.at(airIdx).controlPointList.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(correctedStart.mp.row) + rowStep*idx),
   //                                                                            floor(static_cast<double>(correctedStart.mp.col) + colStep*idx),
   //                                                                            rawdata));
   //      }
   //      airRoutes.at(airIdx).controlPointList.emplace_back(correctedFinish);
   //      airRoutes.at(airIdx).finish = FLYCRCHCKPOINT(finish.mp.row, finish.mp.col, rawdata);
   //   }
   //}
   //else
   //   Q_ASSERT(false);   
   return true;
}

// WARNING: распараллелено!!!
void PathFinder::FindPath(strategy_settings settings, std::shared_ptr<route_data>& routeData/*, const std::shared_ptr<SVM::iMatrix<SurfaceElement>>& rawdata*/, const path_finder_settings pathFinderSettings, path_finder_statistic& statistic)
{
   //m_statistic = &statistic;
   //m_vmeta = ExperimentMeta{
   //   pathFinderSettings.statFieldIndex,
   //   pathFinderSettings.multithread
   //};
   //m_data = rawdata;
   //fillSupportData(rawdata);
   //qint64 startTime = CURTIME_MS();
   //size_t iterations = 2;
   //std::vector<Route> resultAirRoutes;
   //resultAirRoutes.resize(routeData->airRoutes.size());
   //m_funLandVector.clear();
   //m_appSettings = WFM::GetSharedInstance<Dispatcher>(DBG_DATA)->GetSettings();
   //bool pathFound = false;
   //if (pathFinderSettings.multithread)
   //{      
   //   do
   //   {
   //      const int threadCount = 16;   // NOTE: По количеству логических ядер 8+HT
   //      QThreadPool::globalInstance()->setMaxThreadCount(threadCount);
   //      //m_funAirVector.clear();
   //      resultAirRoutes.clear();
   //      resultAirRoutes.resize(routeData->airRoutes.size());
   //      if (!pathFinderSettings.research)
   //         prepareControlPoint(settings, iterations, routeData->landRoutes, resultAirRoutes, rawdata);
   //      else
   //         resultAirRoutes = routeData->airRoutes;
   //      bool finished = true;
   //      size_t idxDelta = 0;
   //      while (!finished)
   //      {
   //         m_synchronizer.clearFutures();
   //         for (size_t idx = 0; idx < pathFinderSettings.packetSize == 0 ? resultAirRoutes.size() : std::min(pathFinderSettings.packetSize, resultAirRoutes.size() - idxDelta - pathFinderSettings.packetSize); idx++)
   //         {
   //            m_synchronizer.addFuture(run(this, &PathFinder::findAirPath, resultAirRoutes.at(idxDelta + idx), iterations, pathFinderSettings.multithread));
   //            //qDebug() << "added task" << idx;
   //         }

   //         if (pathFinderSettings.packetSize == 0 || idxDelta >= resultAirRoutes.size())
   //            finished = true;
   //         //m_funAirVector.emplace_back(run(this, &PathFinder::findAirPath, resultAirRoutes.at(idx), iterations, multithread));
   //         m_synchronizer.waitForFinished();
   //         //qDebug() << "task list finished";
   //         for (size_t idx = 0; idx < m_synchronizer.futures().count(); idx++)
   //         {
   //            resultAirRoutes.at(idxDelta + idx) = m_synchronizer.futures().at(idx).result();
   //         }
   //         idxDelta += pathFinderSettings.packetSize;
   //      }

   //      if (pathFinderSettings.landPath)
   //      {
   //         auto matrix = buildLandCoverage(settings, resultAirRoutes);

   //         m_funLandVector.clear();
   //         for (size_t idx = 0; idx < routeData.get()->landRoutes.size(); idx++)
   //            m_funLandVector.emplace_back(run(this, &PathFinder::findLandPath, routeData.get()->landRoutes.at(idx), matrix, pathFinderSettings.multithread, &pathFound));
   //         for (size_t idx = 0; idx < m_funLandVector.size(); idx++)
   //         {
   //            m_funLandVector.at(idx).waitForFinished();
   //            routeData.get()->landRoutes.at(idx) = m_funLandVector.at(idx).result();
   //         }
   //      }
   //      else
   //         pathFound = true;

   //      //qDebug() << pathFound << iterations;
   //      iterations++;
   //   }
   //   while(!pathFound);
   //}
   //else
   //{      
   //   do
   //   {
   //      resultAirRoutes.clear();
   //      resultAirRoutes.resize(routeData->airRoutes.size());
   //      if (!pathFinderSettings.research)
   //         prepareControlPoint(settings, iterations, routeData->landRoutes, resultAirRoutes, rawdata);
   //      else
   //         resultAirRoutes = routeData->airRoutes;
   //      for (size_t idx = 0; idx < resultAirRoutes.size(); idx++)
   //         resultAirRoutes.at(idx) = findAirPath(resultAirRoutes.at(idx), iterations, pathFinderSettings.multithread);

   //      if (pathFinderSettings.landPath)
   //      {
   //         auto matrix = buildLandCoverage(settings, resultAirRoutes);

   //         for (size_t idx = 0; idx < routeData.get()->landRoutes.size(); idx++)
   //            routeData.get()->landRoutes.at(idx) = findLandPath(routeData.get()->landRoutes.at(idx), matrix, pathFinderSettings.multithread, &pathFound);
   //      }
   //      else
   //         pathFound = true;

   //      //qDebug() << pathFound << iterations;
   //      iterations++;
   //   }
   //   while (!pathFound);
   //}
   //routeData->airRoutes = std::move(resultAirRoutes);
   //qint64 finishTime = CURTIME_MS();
   //qint64 time = finishTime - startTime;
   //qDebug() << "path finding ended: mt: " << pathFinderSettings.multithread << " :" << time;
   //TaskStat stat{m_vmeta, time};
   //QVariant vdata;
   //vdata.setValue(stat);
   ////WFM::GetSharedInstance<Statistic>(DBG_DATA)->StatUpdateTaskRun(vdata);
   //if (m_appSettings->type == STT::ApplicationRunType::ART_RESEARCH
   //    && m_appSettings->res_settings.type == STT::ResearchType::RT_GEN1)
   //   emit TaskDataUpdate(vdata);
   ////clearSupportData();
   //statistic = *m_statistic;
}

void PathFinder::fillSupportData(/*const std::shared_ptr<SVM::iMatrix<SurfaceElement>> &rawdata*/)
{
   //m_rowCount = rawdata->GetRowCount();
   //m_colCount = rawdata->GetColCount();
   /*XFM::CreateMatrix(m_data, m_rowCount, m_colCount);

   for (size_t ridx = 0; ridx < m_rowCount; ridx++)
   {
      for (size_t cidx = 0; cidx < m_colCount; cidx++)
      {
         auto elem = rawdata->Get(ridx, cidx);
         m_data[ridx][cidx].m_x = elem.x;
         m_data[ridx][cidx].m_y = elem.y;
         m_data[ridx][cidx].m_z = elem.z;
         m_data[ridx][cidx].m_go = elem.go;
         m_data[ridx][cidx].m_fly = elem.fly;
      }
   }*/
}

// NOTE: пока что не нужно
void PathFinder::clearSupportData()
{
   //XFM::DeleteMatrix(m_data);
   //TP_MATRIX_CLEAR(m_data, m_rowCount);
}

route PathFinder::findAirPath(route& route, size_t iterations, bool multithread)
{
   //std::vector<CombinedPoint> exp_route;
   //std::vector<CombinedPoint> waypointList;
   //waypointList.emplace_back(route.start);
   //waypointList.insert(waypointList.end(), route.controlPointList.begin(), route.controlPointList.end());
   //waypointList.emplace_back(route.finish);
   //Q_ASSERT(waypointList.size() >= 2);
   //Q_UNUSED(iterations);
   //for (auto item : waypointList)
   //{
   //   auto coord = m_data->Get(item.mp.row, item.mp.col);
   //   item.rp.x = coord.x;
   //   item.rp.y = coord.y;
   //   item.rp.z = coord.z;
   //}
   //aff_checker checker = [](std::shared_ptr<SVM::iMatrix<SurfaceElement>>& data, std::shared_ptr<SVM::iMatrix<size_t>>& coverageMatrix, size_t row, size_t col) -> bool
   //{
   //   Q_UNUSED(coverageMatrix);
   //   return data->Get(row, col).fly.fza == FZA_FORBIDDEN;
   //};
   //height_corrector corrector = [](float y) -> float
   //{
   //   return y + 50.f;
   //};
   //PathFinderLogic logic = { checker, corrector };
   //std::string wplist;
   //for (auto wp : waypointList)
   //   wplist.append(std::string("[" + std::to_string(wp.mp.row) + ";" + std::to_string(wp.mp.col) + "]").c_str());
   ///*THREADDEBUG("st-fn: "
   //         << std::string("[" + std::to_string(route.start.mp.row) + ";" + std::to_string(route.start.mp.col) + "]").c_str()
   //         << std::string("[" + std::to_string(route.finish.mp.row) + ";" + std::to_string(route.finish.mp.col) + "]").c_str());
   //THREADDEBUG("wp list: " << waypointList.size() << " elems: " << wplist.c_str());*/
   //exp_route.emplace_back(waypointList.at(0));
   //for (size_t idx = 0; idx < waypointList.size() - 1; idx++)
   //{
   //   bool found = false;
   //   auto matrix = WFM::CreateSharedObject<SVM::iMatrix<size_t>>(m_rowCount, m_colCount, 0);
   //   std::vector<CombinedPoint> path = findPath(waypointList.at(idx), waypointList.at(idx + 1), logic, matrix, multithread, &found);
   //   if (!found)
   //      Q_ASSERT(false);
   //   //exp_route.clear();
   //   std::reverse(path.begin(), path.end());
   //   //exp_route.emplace_back(waypointList.at(idx));
   //   exp_route.back().rp.y += 50.f;
   //   if (path.size() != 0)
   //   {
   //      exp_route.insert(exp_route.end(), ++path.begin(), path.end());
   //      //exp_route.emplace_back(waypointList.at(idx + 1));
   //      exp_route.back().rp.y += 50.f;
   //   }
   //}
   ////THREADDEBUG("exproute size air: " << exp_route.size());
   //route.route = exp_route;
   return route;
}

route PathFinder::findLandPath(route& route, std::shared_ptr<Matrix<size_t>>& coverageMatrix, bool multithread, bool* pathFounded)
{
   std::vector<SVCG::route_point> exp_route;
   std::vector<SVCG::route_point> waypointList;
   waypointList.emplace_back(route.start);
   waypointList.insert(waypointList.end(), route.control_point_list.begin(), route.control_point_list.end());
   waypointList.emplace_back(route.finish);
   ATLASSERT(waypointList.size() >= 2);
   aff_checker checker = [](std::shared_ptr<Matrix<SVCG::route_point>>& data, std::shared_ptr<Matrix<size_t>>& coverageMatrix, size_t row, size_t col) -> bool
   {
      return data->Get(row, col).go.gza == GoZoneAffilation::GZA_FORBIDDEN || coverageMatrix->Get(row, col) == 0;
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
      std::vector<SVCG::route_point> path = findPath(waypointList.at(idx), waypointList.at(idx + 1), logic, coverageMatrix, multithread, pathFounded);
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

std::vector<SVCG::route_point> PathFinder::findPath(SVCG::route_point& start, SVCG::route_point& finish, path_finder_logic& logic/*, std::shared_ptr<SVM::iMatrix<size_t>>& coverageMatrix*/, bool multithread, bool* pathFound)
{
   std::vector<SVCG::route_point> exp_route;
   /*auto countDist = [](RoutePoint& p1, RoutePoint& p2)->double
   {
       return sqrt(pow(p2.x - p1.x, 2.) + pow(p2.z - p1.z, 2.));
   };*/
   //double maxDist = countDist(start.rp, finish.rp);

   if (start.mp.row == finish.mp.row
       && start.mp.col == finish.mp.col)
   {
      auto point = CombinedPoint{ RoutePoint(start.rp.x,
                                             logic.heightCorrector(start.rp.y),
                                             start.rp.z,
                                             m_data->Get(start.mp.row, start.mp.col).fly.fza,
                                             m_data->Get(start.mp.row, start.mp.col).go.gza), MatrixPoint(start.mp.row, start.mp.col) };
      exp_route.emplace_back(point);
      exp_route.emplace_back(point);
      *pathFound = true;
      return exp_route;
   }

   //auto isInRowSafeRange = [this](int rowIdx)->bool
   //{
   //   return rowIdx >= 0 && rowIdx < static_cast<int>(m_rowCount);
   //};
   //auto isInColSafeRange = [this](int colIdx)->bool
   //{
   //   return colIdx >= 0 && colIdx < static_cast<int>(m_colCount);
   //};

   //qint64 beginFullTime = CURTIME_MS();
   //qint64 beginCreateMatrixTime = CURTIME_MS();
   //// NOTE: волновая матрица
   //auto pointScore = WFM::CreateSharedObject<SVM::iMatrix<size_t>>(m_rowCount, m_colCount, 0);
   ////XFM::CreateMatrix(m_pointScore, m_rowCount, m_colCount);
   ////TP_MATRIX_CREATE(pointScore, m_rowCount, m_colCount, size_t);
   //qint64 endCreateMatrixTime = CURTIME_MS();

   //QVector<QPair<size_t, size_t>> frontline = { { start.mp.row, start.mp.col } };
   //size_t stCurRow = frontline.at(0).first;
   //size_t stCurCol = frontline.at(0).second;

   //for (size_t ridx = 0; ridx < pointScore->GetRowCount(); ridx++)
   //   for (size_t cidx = 0; cidx < pointScore->GetColCount(); cidx++)
   //      pointScore->Set(ridx, cidx, logic.affilationChecker(m_data, coverageMatrix, ridx, cidx) ? /*ULONG_LONG_MAX*/ULLONG_MAX : 0);
   //pointScore->Set(stCurRow, stCurCol, 1);
   ////m_pointScore[stCurRow][stCurCol] = 1;
   //bool find = false;
   //bool traverse = true;
   //size_t frow = finish.mp.row, fcol = finish.mp.col;
   //size_t pointScoreCount = 0;

   //qint64 beginMatrixBuildTime = CURTIME_MS();
   //// NOTE: построение фронта волны
   //while(!find && traverse)
   //{
   //   traverse = false;
   //   QVector<QPair<size_t, size_t>> newFrontline;
   //   for (auto idx = 0; idx < frontline.size(); idx++)
   //   {
   //      size_t curRow = frontline.at(idx).first;
   //      size_t curCol = frontline.at(idx).second;
   //      size_t score =/* m_pointScore[curRow][curCol];*/pointScore->Get(curRow, curCol);
   //      //qDebug() << "Score: " << score << " size " << frontline.size() << " row: " << curRow << " col: " << curCol;
   //      QVector<QPair<size_t, size_t>> pts;
   //      if (isInRowSafeRange(static_cast<int>(curRow) - 1))
   //         pts.push_back({curRow - 1, curCol});
   //      if (isInColSafeRange(static_cast<int>(curCol) - 1))
   //         pts.push_back({curRow, curCol - 1});
   //      if (isInRowSafeRange(static_cast<int>(curRow) + 1))
   //         pts.push_back({curRow + 1, curCol});
   //      if (isInColSafeRange(static_cast<int>(curCol) + 1))
   //         pts.push_back({curRow, curCol + 1});
   //      for (auto iter : pts)
   //      {            
   //         if (pointScore->Get(iter.first, iter.second) == 0)
   //         //if (m_pointScore[iter.first][iter.second] == 0)
   //         {
   //            //pointScoreCount++;
   //            if (logic.affilationChecker(m_data, coverageMatrix, iter.first, iter.second))
   //            {
   //               pointScore->Set(iter.first, iter.second, /*ULONG_LONG_MAX*/ULLONG_MAX);
   //               //m_pointScore[iter.first][iter.second] = ULONG_LONG_MAX;
   //               pointScoreCount++;
   //            }
   //            else
   //            {
   //               newFrontline.push_back({iter.first, iter.second});
   //               pointScore->Set(iter.first, iter.second, score + 1);
   //               //m_pointScore[iter.first][iter.second] = score + 1;
   //               pointScoreCount++;
   //               traverse = true;
   //               if (frow == iter.first && fcol == iter.second)
   //               {
   //                  find = true;
   //                  break;
   //               }
   //            }               
   //         }
   //      }
   //   }
   //   frontline = newFrontline;
   //}
   //qint64 endMatrixBuildTime = CURTIME_MS();
   ////qDebug() << pointScoreCount;

   //*pathFound = find;
   //// NOTE: вариант, когда матрица не добралась до финиша - пути точно нет
   //if (!find)
   //   return exp_route;
   //// ===

   //find = false;
   //QPair<size_t, size_t> curPoint = { finish.mp.row, finish.mp.col };
   //size_t curScore = pointScore->Get(finish.mp.row, finish.mp.col);
   ////size_t curScore = m_pointScore[finish.mp.row][finish.mp.col];
   //size_t curRow = curPoint.first;
   //size_t curCol = curPoint.second;
   //size_t stRow = start.mp.row;
   //size_t stCol = start.mp.col;
   ////size_t pathRecoverCount = 0;
   //auto dist = [](size_t row1, size_t col1, size_t row2, size_t col2)->double
   //{
   //   return sqrt(pow(static_cast<double>(row2) - static_cast<double>(row1), 2.) + pow(static_cast<double>(col2) - static_cast<double>(col1), 2.));
   //};
   //qint64 beginBackfireTime = CURTIME_MS();
   //// NOTE: обратка
   //while(!find)
   //{
   //   find = true;
   //   if (stRow == curRow && stCol == curCol)
   //      break;
   //   auto rp = m_data->Get(curRow, curCol);
   //   CombinedPoint current = { RoutePoint(rp.x,
   //                             logic.heightCorrector(rp.y),
   //                             rp.z,
   //                             rp.fly.fza,
   //                             rp.go.gza),
   //                             MatrixPoint(curRow, curCol) };
   //   //qDebug() << "Route point: " << current.mp.col << current.mp.row;
   //   exp_route.emplace_back(current);
   //   QVector<QPair<double, QPair<size_t, size_t>>> pts;
   //   if (isInRowSafeRange(curRow))
   //      pts.push_back({dist(stRow, stCol, curRow - 1, curCol), {curRow - 1, curCol}});
   //   if (isInColSafeRange(curCol))
   //      pts.push_back({dist(stRow, stCol, curRow, curCol - 1), {curRow, curCol - 1}});
   //   if (isInRowSafeRange(curRow))
   //      pts.push_back({dist(stRow, stCol, curRow + 1, curCol), {curRow + 1, curCol}});
   //   if (isInColSafeRange(curCol))
   //      pts.push_back({dist(stRow, stCol, curRow, curCol + 1), {curRow, curCol + 1}});
   //   std::sort(pts.begin(), pts.end(), [](QPair<double, QPair<size_t, size_t>> r1, QPair<double, QPair<size_t, size_t>> r2)->bool { return r1.first < r2.first; });
   //   //std::sort(pts.begin(), pts.end(), [](QPair<double, QPair<size_t, size_t>> r1, QPair<double, QPair<size_t, size_t>> r2)->bool { return r1.first > r2.first; });

   //   for (auto iter : pts)
   //   {
   //      if (pointScore->Get(iter.second.first, iter.second.second) != 0
   //          && pointScore->Get(iter.second.first, iter.second.second) < curScore)
   //      /*if (m_pointScore[iter.second.first][iter.second.second] != 0
   //          && m_pointScore[iter.second.first][iter.second.second] < curScore)*/
   //      {
   //         curRow = iter.second.first;
   //         curCol = iter.second.second;
   //         curScore = pointScore->Get(curRow, curCol);
   //         //curScore = m_pointScore[curRow][curCol];
   //         //qDebug() << "cs: " << curScore;
   //         find = false;
   //         break;
   //      }
   //   }
   //}
   //auto rp = m_data->Get(curRow, curCol);
   //exp_route.emplace_back(CombinedPoint{ RoutePoint(rp.x,
   //                                    logic.heightCorrector(rp.y),
   //                                    rp.z,
   //                                    rp.fly.fza,
   //                                    rp.go.gza), MatrixPoint(curRow, curCol) });
   //qint64 endBackfireTime = CURTIME_MS();
   //qint64 endFullTime = CURTIME_MS();
   //qint64 fullTime = endFullTime - beginFullTime;
   //qint64 crTime = endCreateMatrixTime - beginCreateMatrixTime;
   //qint64 bldTime = endMatrixBuildTime - beginMatrixBuildTime;
   //qint64 bfTime = endBackfireTime - beginBackfireTime;
   ///*qDebug() << "full:" << fullTime;
   //qDebug() << "cr:" << crTime;
   //qDebug() << "bld:" << bldTime;
   //qDebug() << "backfire:" << bfTime;*/
   ////Q_ASSERT();
   ////TP_MATRIX_CLEAR(pointScore, m_rowCount);
   //SubTaskStat data{m_vmeta, fullTime, crTime, bldTime, bfTime, pointScoreCount};
   //QVariant vdata;
   //vdata.setValue(data);
   ////qDebug() << "fp run";
   ////WFM::GetSharedInstance<Statistic>(DBG_DATA)->StatUpdateSubTaskRun(vdata);
   //if (m_appSettings->type == STT::ApplicationRunType::ART_RESEARCH
   //    && m_appSettings->res_settings.type == STT::ResearchType::RT_GEN1)
   //   emit SubTaskDataUpdate(vdata);
   return exp_route;
}
