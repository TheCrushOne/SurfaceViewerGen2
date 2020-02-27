#include "StrategyManager.h"


#define FLYCRCHCKPOINT(row, col, data) CombinedPoint{ RoutePoint(0.f, 0.f, 0.f, FZA_NORMAL, GZA_NORMAL), \
                        checkRetranslateFlyPointAffilation(static_cast<int>(row), static_cast<int>(col), data) }

bool StrategyManager::PrepareControlPoint(pathfinder::strategy_settings settings, size_t iterations, std::vector<route>& landRoutes, std::vector<pathfinder::route>& airRoutes, const std::shared_ptr<pathfinder::Matrix<SVCG::route_point>>& rawdata)
{
   pathfinder::StrategyType type = settings.type;
   auto start = landRoutes.at(0).start, finish = landRoutes.at(0).finish;
   //bool finished = false;
   int radius = static_cast<int>(settings.radius);
   int diameter = 2*radius;
   bool isPlusDirectionRow = start.mp.row < finish.mp.row;
   bool isPlusDirectionCol = start.mp.col < finish.mp.col;
   size_t rowCount = rawdata->GetRowCount(), colCount = rawdata->GetColCount();
   size_t directionControllerPointCount = 20;

   if (type == pathfinder::StrategyType::ST_SNAKE)
   {
      // NOTE: у этой стратегии не может быть больше одной ходки(полное покрытие)
      //ATLASSERT(iterations == 1);
      bool fnDirection = true;
      int currentRowStLine;
      int currentColStLine;
      int currentRowFnLine;
      int currentColFnLine;
      bool first = true;
      // NOTE: количество ходок
      size_t directionCount = static_cast<size_t>(abs(static_cast<int>(rowCount))/(diameter*static_cast<int>(airRoutes.size()))) + 2;   // WARNING: костыль
      size_t droneCount = airRoutes.size();
      // NOTE: отступ от границ карты
      //int startBorderOffset = radius;

      for (size_t airIdx = 0; airIdx < airRoutes.size(); airIdx++)
      {
         currentRowStLine = static_cast<int>(start.mp.row);
         currentColStLine = static_cast<int>(start.mp.col);
         currentRowFnLine = static_cast<int>(start.mp.row);
         currentColFnLine = static_cast<int>(finish.mp.col);

         airRoutes.at(airIdx).start = FLYCRCHCKPOINT(start.mp.row, start.mp.col, rawdata);
         //DMPCORR(start.mp.row, start.mp.col, airRoutes.at(airIdx).start);

         for (size_t dirIdx = 0; dirIdx < directionCount; dirIdx++)
         {
            // NOTE: эквивалент(логическая операция)
            fnDirection = (dirIdx % 2 == 0) == (isPlusDirectionCol);
            if (fnDirection)
            {
               // TODO: добить
               currentRowStLine = static_cast<int>(dirIdx*droneCount)*diameter + static_cast<int>(airIdx)*diameter + radius;// (fnDirection ? 1 : -1)*(isPlusDirectionRow ? 1 : -1)*shift;
               currentColStLine = static_cast<int>(droneCount - airIdx - 1)*diameter + radius;
               currentRowFnLine = currentRowStLine;
               currentColFnLine = static_cast<int>(colCount) - static_cast<int>(airIdx)*diameter - radius;
               //currentRowFnLine = (fnDirection ? 1 : -1)*(isPlusDirectionRow ? 1 : -1)*shift;
               //if (!first)
                  //currentColStLine += (fnDirection ? 1 : -1)*(isPlusDirectionCol ? 1 : -1)*shift;
               //currentColFnLine += (fnDirection ? 1 : -1)*(isPlusDirectionCol ? -1 : 1)*shift;
            }
            else
            {
               currentRowStLine = static_cast<int>(dirIdx*droneCount)*diameter + static_cast<int>(droneCount - airIdx - 1)*diameter + radius;
               currentColStLine = static_cast<int>(colCount) - static_cast<int>(airIdx)*diameter - radius;
               currentRowFnLine = currentRowStLine;// (fnDirection ? 1 : -1)*(isPlusDirectionRow ? 1 : -1)*shift;
               currentColFnLine = static_cast<int>(droneCount - airIdx - 1)*diameter + radius;
            }
            auto correctedStart = FLYCRCHCKPOINT(currentRowStLine, currentColStLine, rawdata);
            auto correctedFinish = FLYCRCHCKPOINT(currentRowFnLine, currentColFnLine, rawdata);
            DMPCORR(currentRowStLine, currentColStLine, correctedStart);
            DMPCORR(currentRowFnLine, currentColFnLine, correctedFinish);
            airRoutes.at(airIdx).controlPointList.emplace_back(correctedStart);

            double rowStep = (static_cast<double>(correctedFinish.mp.row) - static_cast<double>(correctedStart.mp.row))/static_cast<double>(directionControllerPointCount);
            double colStep = (static_cast<double>(correctedFinish.mp.col) - static_cast<double>(correctedStart.mp.col))/static_cast<double>(directionControllerPointCount);
            for (size_t idx = 1; idx < directionControllerPointCount; idx++)
            {
               auto correctedPoint = FLYCRCHCKPOINT(floor(static_cast<double>(correctedStart.mp.row) + rowStep*idx)
                                                  , floor(static_cast<double>(correctedStart.mp.col) + colStep*idx)
                                                  , rawdata);
               airRoutes.at(airIdx).controlPointList.emplace_back(correctedPoint);
               DMPCORR(floor(static_cast<double>(correctedStart.mp.row) + rowStep*idx)
                     , floor(static_cast<double>(correctedStart.mp.col) + colStep*idx)
                     , correctedPoint);
            }

            airRoutes.at(airIdx).controlPointList.emplace_back(correctedFinish);
         }

         airRoutes.at(airIdx).finish = FLYCRCHCKPOINT(finish.mp.row, finish.mp.col, rawdata);
         DMPCORR(finish.mp.row, finish.mp.col, airRoutes.at(airIdx).finish);
      }

      first = false;
   }
   else if (type == ST_SECTOR)
   {
      airRoutes.at(0).start = FLYCRCHCKPOINT(start.mp.row, start.mp.col, rawdata);
      double rowStep = (static_cast<double>(finish.mp.row) - static_cast<double>(start.mp.row))/static_cast<double>(directionControllerPointCount);
      double colStep = (static_cast<double>(finish.mp.col) - static_cast<double>(start.mp.col))/static_cast<double>(directionControllerPointCount);
      for (size_t idx = 1; idx < directionControllerPointCount; idx++)
      {
         airRoutes.at(0).controlPointList.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(start.mp.row) + rowStep*idx),
                                                                            floor(static_cast<double>(start.mp.col) + colStep*idx),
                                                                            rawdata));
      }
      airRoutes.at(0).finish = FLYCRCHCKPOINT(finish.mp.row, finish.mp.col, rawdata);

      if (iterations == 1 && airRoutes.size() == 3)
         int i = 0;

      for (size_t lineIdx = 1; lineIdx < airRoutes.size()*iterations; lineIdx++)
      {
         size_t airIdx = lineIdx % airRoutes.size();
         airRoutes.at(airIdx).start = FLYCRCHCKPOINT(start.mp.row, start.mp.col, rawdata);
         int finishRow, finishCol;
         if (lineIdx % 2 == 0)
         {
            finishRow = static_cast<int>(finish.mp.row) + (static_cast<int>(lineIdx) / 2)*(isPlusDirectionRow ? -1 : 1)*diameter;
            finishCol = static_cast<int>(finish.mp.col);
         }
         else
         {
            finishRow = static_cast<int>(finish.mp.row);
            finishCol = static_cast<int>(finish.mp.col) + (static_cast<int>(lineIdx) / 2 + 1)*(isPlusDirectionCol ? -1 : 1)*diameter;
         }
         auto correctedFinish = FLYCRCHCKPOINT(finishRow, finishCol, rawdata);
         double rowStep = (static_cast<double>(correctedFinish.mp.row) - static_cast<double>(start.mp.row))/static_cast<double>(directionControllerPointCount);
         double colStep = (static_cast<double>(correctedFinish.mp.col) - static_cast<double>(start.mp.col))/static_cast<double>(directionControllerPointCount);
         for (size_t idx = 1; idx < directionControllerPointCount; idx++)
         {
            airRoutes.at(airIdx).controlPointList.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(start.mp.row) + rowStep*idx),
                                                                               floor(static_cast<double>(start.mp.col) + colStep*idx),
                                                                               rawdata));
         }
         airRoutes.at(airIdx).controlPointList.emplace_back(correctedFinish);
         airRoutes.at(airIdx).finish = FLYCRCHCKPOINT(finish.mp.row, finish.mp.col, rawdata);
      }
   }
   else if (type == ST_RHOMBOID)
   {
      //airRoutes.at(0).controlPointList.clear();
      airRoutes.at(0).start = FLYCRCHCKPOINT(start.mp.row, start.mp.col, rawdata);
      double rowStep = (static_cast<double>(finish.mp.row) - static_cast<double>(start.mp.row))/static_cast<double>(directionControllerPointCount);
      double colStep = (static_cast<double>(finish.mp.col) - static_cast<double>(start.mp.col))/static_cast<double>(directionControllerPointCount);
      for (size_t idx = 1; idx < directionControllerPointCount; idx++)
      {
         airRoutes.at(0).controlPointList.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(start.mp.row) + rowStep*idx),
                                                                            floor(static_cast<double>(start.mp.col) + colStep*idx),
                                                                            rawdata));
      }
      airRoutes.at(0).finish = FLYCRCHCKPOINT(finish.mp.row, finish.mp.col, rawdata);

      for (size_t lineIdx = 1; lineIdx < airRoutes.size()*iterations; lineIdx++)
      {
         size_t airIdx = lineIdx % airRoutes.size();
         //airRoutes.at(airIdx).controlPointList.clear();
         airRoutes.at(airIdx).start = FLYCRCHCKPOINT(start.mp.row, start.mp.col, rawdata);
         int finishRow, finishCol, startRow, startCol;
         if (lineIdx % 2 == 0)
         {
            startRow = static_cast<int>(start.mp.row) + (static_cast<int>(lineIdx) / 2)*(isPlusDirectionRow ? 1 : -1)*diameter;
            startCol = static_cast<int>(start.mp.col);
            finishRow = static_cast<int>(finish.mp.row);
            finishCol = static_cast<int>(finish.mp.col) - (static_cast<int>(lineIdx) / 2)*(isPlusDirectionCol ? 1 : -1)*diameter;
         }
         else
         {
            startRow = static_cast<int>(start.mp.row);
            startCol = static_cast<int>(start.mp.col) + (static_cast<int>(lineIdx) / 2 + 1)*(isPlusDirectionCol ? 1 : -1)*diameter;
            finishRow = static_cast<int>(finish.mp.row) - (static_cast<int>(lineIdx) / 2 + 1)*(isPlusDirectionRow ? 1 : -1)*diameter;
            finishCol = static_cast<int>(finish.mp.col);
         }
         auto correctedStart = FLYCRCHCKPOINT(startRow, startCol, rawdata);
         auto correctedFinish = FLYCRCHCKPOINT(finishRow, finishCol, rawdata);
         airRoutes.at(airIdx).controlPointList.emplace_back(correctedStart);
         double rowStep = (static_cast<double>(correctedFinish.mp.row) - static_cast<double>(correctedStart.mp.row))/static_cast<double>(directionControllerPointCount);
         double colStep = (static_cast<double>(correctedFinish.mp.col) - static_cast<double>(correctedStart.mp.col))/static_cast<double>(directionControllerPointCount);
         for (size_t idx = 1; idx < directionControllerPointCount; idx++)
         {
            airRoutes.at(airIdx).controlPointList.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(correctedStart.mp.row) + rowStep*idx),
                                                                               floor(static_cast<double>(correctedStart.mp.col) + colStep*idx),
                                                                               rawdata));
         }
         airRoutes.at(airIdx).controlPointList.emplace_back(correctedFinish);
         airRoutes.at(airIdx).finish = FLYCRCHCKPOINT(finish.mp.row, finish.mp.col, rawdata);
      }
   }
   else
      Q_ASSERT(false);   
   return true;
}
