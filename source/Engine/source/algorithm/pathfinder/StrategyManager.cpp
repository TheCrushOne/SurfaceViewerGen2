#include "StrategyManager.h"
#include "main/engine.h"

#define FLYCRCHCKPOINT(row, col, data) checkRetranslateFlyPointAffilation(static_cast<int>(row), static_cast<int>(col), data)


#define DMPCORR(oldrow, oldcol, nwp) if (nwp.row != oldrow || nwp.col != oldcol) \
   Message(ICommunicator::MS_Info, "Corr: [ %i, %i ] -> [ %i, %i ]", oldrow, oldcol, nwp.row, nwp.col);


using namespace pathfinder;

bool StrategyManager::PrepareControlPoint(pathfinder::strategy_settings settings, size_t iterations, std::vector<settings::route>& landRoutes, std::vector<settings::route>& airRoutes, const std::shared_ptr<Matrix<SVCG::route_point>>& rawdata)
{
   pathfinder::StrategyType type = settings.type;
   auto start = landRoutes.at(0).start, finish = landRoutes.at(0).finish;
   //bool finished = false;
   int radius = static_cast<int>(settings.radius);
   int diameter = 2*radius;
   bool isPlusDirectionRow = start.row < finish.row;
   bool isPlusDirectionCol = start.col < finish.col;
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
         currentRowStLine = static_cast<int>(start.row);
         currentColStLine = static_cast<int>(start.col);
         currentRowFnLine = static_cast<int>(start.row);
         currentColFnLine = static_cast<int>(finish.col);

         airRoutes.at(airIdx).start = FLYCRCHCKPOINT(start.row, start.col, rawdata);
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
            airRoutes.at(airIdx).control_point_list.emplace_back(correctedStart);

            double rowStep = (static_cast<double>(correctedFinish.row) - static_cast<double>(correctedStart.row))/static_cast<double>(directionControllerPointCount);
            double colStep = (static_cast<double>(correctedFinish.col) - static_cast<double>(correctedStart.col))/static_cast<double>(directionControllerPointCount);
            for (size_t idx = 1; idx < directionControllerPointCount; idx++)
            {
               auto correctedPoint = FLYCRCHCKPOINT(floor(static_cast<double>(correctedStart.row) + rowStep*idx)
                                                  , floor(static_cast<double>(correctedStart.col) + colStep*idx)
                                                  , rawdata);
               airRoutes.at(airIdx).control_point_list.emplace_back(correctedPoint);
               DMPCORR(floor(static_cast<double>(correctedStart.row) + rowStep*idx)
                     , floor(static_cast<double>(correctedStart.col) + colStep*idx)
                     , correctedPoint);
            }

            airRoutes.at(airIdx).control_point_list.emplace_back(correctedFinish);
         }

         airRoutes.at(airIdx).finish = FLYCRCHCKPOINT(finish.row, finish.col, rawdata);
         DMPCORR(finish.row, finish.col, airRoutes.at(airIdx).finish);
      }

      first = false;
   }
   else if (type == ST_SECTOR)
   {
      airRoutes.at(0).start = FLYCRCHCKPOINT(start.row, start.col, rawdata);
      double rowStep = (static_cast<double>(finish.row) - static_cast<double>(start.row))/static_cast<double>(directionControllerPointCount);
      double colStep = (static_cast<double>(finish.col) - static_cast<double>(start.col))/static_cast<double>(directionControllerPointCount);
      for (size_t idx = 1; idx < directionControllerPointCount; idx++)
      {
         airRoutes.at(0).control_point_list.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(start.row) + rowStep*idx),
                                                                            floor(static_cast<double>(start.col) + colStep*idx),
                                                                            rawdata));
      }
      airRoutes.at(0).finish = FLYCRCHCKPOINT(finish.row, finish.col, rawdata);

      if (iterations == 1 && airRoutes.size() == 3)
         int i = 0;

      for (size_t lineIdx = 1; lineIdx < airRoutes.size()*iterations; lineIdx++)
      {
         size_t airIdx = lineIdx % airRoutes.size();
         airRoutes.at(airIdx).start = FLYCRCHCKPOINT(start.row, start.col, rawdata);
         int finishRow, finishCol;
         if (lineIdx % 2 == 0)
         {
            finishRow = static_cast<int>(finish.row) + (static_cast<int>(lineIdx) / 2)*(isPlusDirectionRow ? -1 : 1)*diameter;
            finishCol = static_cast<int>(finish.col);
         }
         else
         {
            finishRow = static_cast<int>(finish.row);
            finishCol = static_cast<int>(finish.col) + (static_cast<int>(lineIdx) / 2 + 1)*(isPlusDirectionCol ? -1 : 1)*diameter;
         }
         auto correctedFinish = FLYCRCHCKPOINT(finishRow, finishCol, rawdata);
         double rowStep = (static_cast<double>(correctedFinish.row) - static_cast<double>(start.row))/static_cast<double>(directionControllerPointCount);
         double colStep = (static_cast<double>(correctedFinish.col) - static_cast<double>(start.col))/static_cast<double>(directionControllerPointCount);
         for (size_t idx = 1; idx < directionControllerPointCount; idx++)
         {
            airRoutes.at(airIdx).control_point_list.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(start.row) + rowStep*idx),
                                                                               floor(static_cast<double>(start.col) + colStep*idx),
                                                                               rawdata));
         }
         airRoutes.at(airIdx).control_point_list.emplace_back(correctedFinish);
         airRoutes.at(airIdx).finish = FLYCRCHCKPOINT(finish.row, finish.col, rawdata);
      }
   }
   else if (type == ST_RHOMBOID)
   {
      //airRoutes.at(0).controlPointList.clear();
      airRoutes.at(0).start = FLYCRCHCKPOINT(start.row, start.col, rawdata);
      double rowStep = (static_cast<double>(finish.row) - static_cast<double>(start.row))/static_cast<double>(directionControllerPointCount);
      double colStep = (static_cast<double>(finish.col) - static_cast<double>(start.col))/static_cast<double>(directionControllerPointCount);
      for (size_t idx = 1; idx < directionControllerPointCount; idx++)
      {
         airRoutes.at(0).control_point_list.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(start.row) + rowStep*idx),
                                                                        floor(static_cast<double>(start.col) + colStep*idx),
                                                                        rawdata));
      }
      airRoutes.at(0).finish = FLYCRCHCKPOINT(finish.row, finish.col, rawdata);

      for (size_t lineIdx = 1; lineIdx < airRoutes.size()*iterations; lineIdx++)
      {
         size_t airIdx = lineIdx % airRoutes.size();
         //airRoutes.at(airIdx).controlPointList.clear();
         airRoutes.at(airIdx).start = FLYCRCHCKPOINT(start.row, start.col, rawdata);
         int finishRow, finishCol, startRow, startCol;
         if (lineIdx % 2 == 0)
         {
            startRow = static_cast<int>(start.row) + (static_cast<int>(lineIdx) / 2)*(isPlusDirectionRow ? 1 : -1)*diameter;
            startCol = static_cast<int>(start.col);
            finishRow = static_cast<int>(finish.row);
            finishCol = static_cast<int>(finish.col) - (static_cast<int>(lineIdx) / 2)*(isPlusDirectionCol ? 1 : -1)*diameter;
         }
         else
         {
            startRow = static_cast<int>(start.row);
            startCol = static_cast<int>(start.col) + (static_cast<int>(lineIdx) / 2 + 1)*(isPlusDirectionCol ? 1 : -1)*diameter;
            finishRow = static_cast<int>(finish.row) - (static_cast<int>(lineIdx) / 2 + 1)*(isPlusDirectionRow ? 1 : -1)*diameter;
            finishCol = static_cast<int>(finish.col);
         }
         auto correctedStart = FLYCRCHCKPOINT(startRow, startCol, rawdata);
         auto correctedFinish = FLYCRCHCKPOINT(finishRow, finishCol, rawdata);
         airRoutes.at(airIdx).control_point_list.emplace_back(correctedStart);
         double rowStep = (static_cast<double>(correctedFinish.row) - static_cast<double>(correctedStart.row))/static_cast<double>(directionControllerPointCount);
         double colStep = (static_cast<double>(correctedFinish.col) - static_cast<double>(correctedStart.col))/static_cast<double>(directionControllerPointCount);
         for (size_t idx = 1; idx < directionControllerPointCount; idx++)
         {
            airRoutes.at(airIdx).control_point_list.emplace_back(FLYCRCHCKPOINT(floor(static_cast<double>(correctedStart.row) + rowStep*idx),
                                                                               floor(static_cast<double>(correctedStart.col) + colStep*idx),
                                                                               rawdata));
         }
         airRoutes.at(airIdx).control_point_list.emplace_back(correctedFinish);
         airRoutes.at(airIdx).finish = FLYCRCHCKPOINT(finish.row, finish.col, rawdata);
      }
   }
   else
   {
      //ATLASSERT(false);
   }
   return true;
}


SVCG::route_point StrategyManager::checkRetranslateFlyPointAffilation(int row, int col, const std::shared_ptr<Matrix<SVCG::route_point>>& rawdata)
{
   auto rowCount = rawdata->GetRowCount();
   auto colCount = rawdata->GetColCount();
   auto checkPointBorderCollision = [&rowCount, &colCount](int row, int col)->bool
   {
      return !(row >= 0 && row < static_cast<int>(rowCount) && col >= 0 && col < static_cast<int>(colCount));
   };
   bool /*found = false,*/ stuck = false;
   int step = 1;
   if (checkPointBorderCollision(row, col) || rawdata->Get(static_cast<size_t>(row), static_cast<size_t>(col)).fly == FlyZoneAffilation::FZA_FORBIDDEN)
   {
      while (!stuck)
      {
         if ((row - step < 0)
            && (row + step >= static_cast<int>(rowCount))
            && (col - step < 0)
            && (col + step >= static_cast<int>(colCount)))
         {
            stuck = true;
            //qDebug() << "stucked: " << row << col;
            return SVCG::route_point(0, 0);
         }

         if (!checkPointBorderCollision(row - step, col)
            && (rawdata->Get(static_cast<size_t>(row - step), static_cast<size_t>(col)).fly != FlyZoneAffilation::FZA_FORBIDDEN))
            return SVCG::route_point(row - step, col);

         if (!checkPointBorderCollision(row + step, col)
            && (rawdata->Get(static_cast<size_t>(row + step), static_cast<size_t>(col)).fly != FlyZoneAffilation::FZA_FORBIDDEN))
            return SVCG::route_point(row + step, col);

         if (!checkPointBorderCollision(row, col - step)
            && (rawdata->Get(static_cast<size_t>(row), static_cast<size_t>(col - step)).fly != FlyZoneAffilation::FZA_FORBIDDEN))
            return SVCG::route_point(row, col - step);

         if (!checkPointBorderCollision(row, col + step)
            && (rawdata->Get(static_cast<size_t>(row), static_cast<size_t>(col + step)).fly != FlyZoneAffilation::FZA_FORBIDDEN))
            return SVCG::route_point(row, col + step);
         step++;
         //qDebug() << "corrector step:" << step << row << col;//row << static_cast<int>(rowCount) << (row - step < 0) << (row + step >= static_cast<int>(rowCount)) << (col - step < 0) << (col + step >= static_cast<int>(colCount));
      }
   }
   return SVCG::route_point(row, col, 0.f);
}