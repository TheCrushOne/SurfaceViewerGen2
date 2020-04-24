#include "stdafx.h"
#include "CoordinateCorrectionHelper.h"

SVCG::route_point CoordinateCorrectionHelper::CorrectPoint(const std::shared_ptr<pathfinder::Matrix<SVCG::route_point>>& rawdata, int row, int col, affilationCheckerMtd checker, ICommunicator* communicator)
{
   auto rowCount = rawdata->GetRowCount();
   auto colCount = rawdata->GetColCount();
   auto checkPointBorderCollision = [&rowCount, &colCount](int row, int col)->bool
   {
      return !(row >= 0 && row < static_cast<int>(rowCount) && col >= 0 && col < static_cast<int>(colCount));
   };
   if (checkPointBorderCollision(row, col))
   {
      if (row < 0)
         row = 0;
      if (row > static_cast<int>(rowCount))
         row = static_cast<int>(rowCount) - 1;
      if (col < 0)
         col = 0;
      if (col > static_cast<int>(colCount))
         col = static_cast<int>(colCount) - 1;
   }
   bool found = false, stuck = false;
   int step = 1;
   int correctedRow = 0, correctedCol = 0;

   auto isInRowSafeRange = [rawdata](int rowIdx)->bool
   {
      return rowIdx >= 0 && rowIdx < static_cast<int>(rawdata->GetRowCount());
   };
   auto isInColSafeRange = [rawdata](int colIdx)->bool
   {
      return colIdx >= 0 && colIdx < static_cast<int>(rawdata->GetColCount());
   };
   auto frontlineCheckEmplace = [isInRowSafeRange, isInColSafeRange](std::vector<std::pair<size_t, size_t>>& storage, size_t row, size_t col)
   {
      if (isInRowSafeRange(static_cast<int>(row)) && isInColSafeRange(static_cast<int>(col)))
         storage.emplace_back(std::make_pair(row, col));
   };

   if (rawdata->Get(static_cast<size_t>(row), static_cast<size_t>(col)).fly == pathfinder::FlyZoneAffilation::FZA_FORBIDDEN)
   {
      auto pointScore = std::make_shared<pathfinder::Matrix<size_t>>(rawdata->GetRowCount(), rawdata->GetColCount(), 0);
      std::vector<std::pair<size_t, size_t>> frontline = { { row, col } };
      while (!stuck && !found)
      {
         std::vector<std::pair<size_t, size_t>> newFrontline;
         for (size_t idx = 0; idx < frontline.size(); idx++)
         {
            size_t curRow = frontline.at(idx).first;
            size_t curCol = frontline.at(idx).second;
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
                  if (checker(rawdata, iter.first, iter.second))
                  {
                     found = true;
                     correctedRow = iter.first;
                     correctedCol = iter.second;
                     communicator->Message(ICommunicator::MessageType::MT_INFO, "fly control point correction [%i %i] -> [%i %i]", row, col, correctedRow, correctedCol);
                     return SVCG::route_point(correctedRow, correctedCol, 0.f);
                  }
                  else
                  {
                     newFrontline.push_back({ iter.first, iter.second });
                     pointScore->Set(iter.first, iter.second, 1);
                  }
               }
            }
         }
         frontline = newFrontline;

         if (frontline.size() == 0)
         {
            stuck = true;
            communicator->Message(ICommunicator::MessageType::MT_ERROR, "fly control point correction stucked [%i %i]", row, col);
            return SVCG::route_point(0, 0);
         }
         //qDebug() << "corrector step:" << step << row << col;//row << static_cast<int>(rowCount) << (row - step < 0) << (row + step >= static_cast<int>(rowCount)) << (col - step < 0) << (col + step >= static_cast<int>(colCount));
      }
   }
   return SVCG::route_point(row, col);
}