#include "CoverageBuilder.h"

std::shared_ptr<pathfinder::Matrix<size_t>> CoverageBuilder::BuildLandCoverage(size_t rowCount, size_t colCount, pathfinder::strategy_settings settings, std::vector<pathfinder::route>& airRoutes)
{
   auto countDist = [](int x1, int y1, int x2, int y2)->double
   {
      return sqrt(pow(y2 - y1, 2.) + pow(x2 - x1, 2.));
   };
   auto coverageMatrix = std::make_shared<pathfinder::Matrix<size_t>>(rowCount, colCount, 0);
   int radius = static_cast<int>(settings.radius) + 2;   // NOTE: костыль

   for (size_t airIdx = 0; airIdx < airRoutes.size(); airIdx++)
   {
      for (auto cp : airRoutes.at(airIdx).route_list)
      {
         //bool pointInRadius = true;
         //double idxRingStep = 1;
         int lowerRowPoint = (static_cast<int>(cp.row) - radius >= 0 ? cp.row - radius : 0);
         int higherRowPoint = (static_cast<int>(cp.row) + radius < static_cast<int>(rowCount) ? cp.row + radius : static_cast<int>(rowCount) - 1);
         int lowerColPoint = (static_cast<int>(cp.col) - radius >= 0 ? cp.col - radius : 0);
         int higherColPoint = (static_cast<int>(cp.col) + radius < static_cast<int>(colCount) ? cp.col + radius : static_cast<int>(colCount) - 1);
         for (int row = lowerRowPoint; row <= higherRowPoint; row++)
         {
            for (int col = lowerColPoint; col <= higherColPoint; col++)
            {
               if (countDist(row, col, cp.row, cp.col) < radius)
                  coverageMatrix->Set(static_cast<size_t>(row), static_cast<size_t>(col), 1);
            }
         }
      }
   }

   std::string time_format = "ddMMyyyy_HHmm";
   //QDateTime a = QDateTime::currentDateTime();
   //QString as = a.toString(time_format);

   // TODO: подключить нормально и переделать вывод матрицы покрытия
   settings::pathfinding_settings settings;
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

  //auto coverageMatrix = std::make_shared<Matrix<size_t>>(0, 0, 0) ; //WFM::CreateSharedObject<SVM::iMatrix<size_t>>(m_rowCount, m_colCount, 0);
   return coverageMatrix;
}

bool CoverageBuilder::LandPathCoverage(size_t rowCount, size_t colCount, pathfinder::strategy_settings settings, std::vector<pathfinder::route>& landRoutes, std::vector<pathfinder::route>& airRoutes)
{
   auto countDist = [](int x1, int y1, int x2, int y2)->double
   {
       return sqrt(pow(y2 - y1, 2.) + pow(x2 - x1, 2.));
   };
   auto coverageMatrix = std::make_shared<pathfinder::Matrix<size_t>>(rowCount, colCount, 0);
   int radius = static_cast<int>(settings.radius) + 2;   // NOTE: костыль

   for (size_t airIdx = 0; airIdx < airRoutes.size(); airIdx++)
   {
      for (auto cp : airRoutes.at(airIdx).route_list)
      {
         //bool pointInRadius = true;
         //double idxRingStep = 1;
         int lowerRowPoint = (cp.row - radius >= 0 ? cp.row - radius : 0);
         int higherRowPoint = (cp.row + radius < static_cast<int>(rowCount) ? cp.row + radius : static_cast<int>(rowCount) - 1);
         int lowerColPoint = (cp.col - radius >= 0 ? cp.col - radius : 0);
         int higherColPoint = (cp.col + radius < static_cast<int>(colCount) ? cp.col + radius : static_cast<int>(colCount) - 1);
         for (int row = lowerRowPoint; row <= higherRowPoint; row++)
         {
            for (int col = lowerColPoint; col <= higherColPoint; col++)
            {
               if (countDist(row, col, cp.row, cp.col) < radius)
                  coverageMatrix->Set(static_cast<size_t>(row), static_cast<size_t>(col), 1);
            }
         }
      }
   }

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

   for (size_t landIdx = 0; landIdx < landRoutes.size(); landIdx++)
   {
      for (auto cp : landRoutes.at(landIdx).route_list)
      {
         if (coverageMatrix->Get(cp.row, cp.col) == 0)
            return false;
      }
   }

   return true;
}
