#include "stdafx.h"
#include "StrategyManager.h"
#include "main/engine.h"
#include "Helpers\CoordinateCorrectionHelper.h"

// HACK: если геометрия не сходится, можно принять пи равным 7 и проверить, изменилось ли что
#define PI 3.14159265

#define FLYCRCHCKPOINT(row, col, data) checkRetranslateFlyPointAffilation(static_cast<int>(row), static_cast<int>(col), data)

using namespace SV;
using namespace SV::pathfinder;

bool StrategyManager::PrepareControlPoint(size_t iterations, std::vector<settings::route>& landRoutes, std::vector<settings::route>& airRoutes, const std::shared_ptr<RoutePointMatrix>& rawdata, const std::shared_ptr<pathfinder::path_finder_indata> indata)
{
   ATLASSERT(indata->unit_data.land_units.size() > 0);
   pathfinder::StrategyType type = indata->strategy_settings.type;
   auto start = indata->unit_data.land_units.at(0).start, finish = indata->unit_data.land_units.at(0).finish;
   //bool finished = false;
   int radius = static_cast<int>(indata->strategy_settings.radius);
   int diameter = 2*radius;
   bool isPlusDirectionRow = start.row < finish.row;
   bool isPlusDirectionCol = start.col < finish.col;
   size_t rowCount = rawdata->GetRowCount(), colCount = rawdata->GetColCount();
   size_t directionControllerPointCount = 20;
   auto controlPointInserter = [](settings::route& route, const CG::route_point point)
   {
      /*if (route.start == SVCG::route_point())
      {
         route.start = point;
         return;
      }
      if (route.finish == SVCG::route_point())
      {
         route.finish = point;
         return;
      }*/
      route.control_point_list.emplace_back(/*route.finish*/point);
      //route.finish = point;
   };

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
      size_t directionCount = static_cast<size_t>(abs(static_cast<int>(rowCount))/(diameter*static_cast<int>(indata->unit_data.air_units.size()))) + 2;   // WARNING: костыль
      size_t droneCount = indata->unit_data.air_units.size();

      airRoutes.resize(indata->unit_data.air_units.size());
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
            airRoutes.at(airIdx).control_point_list.emplace_back(correctedStart);

            double rowStep = (static_cast<double>(correctedFinish.row) - static_cast<double>(correctedStart.row))/static_cast<double>(directionControllerPointCount);
            double colStep = (static_cast<double>(correctedFinish.col) - static_cast<double>(correctedStart.col))/static_cast<double>(directionControllerPointCount);
            for (size_t idx = 1; idx < directionControllerPointCount; idx++)
            {
               auto correctedPoint = FLYCRCHCKPOINT(floor(static_cast<double>(correctedStart.row) + rowStep*idx)
                                                  , floor(static_cast<double>(correctedStart.col) + colStep*idx)
                                                  , rawdata);
               airRoutes.at(airIdx).control_point_list.emplace_back(correctedPoint);
            }

            airRoutes.at(airIdx).control_point_list.emplace_back(correctedFinish);
         }

         airRoutes.at(airIdx).finish = FLYCRCHCKPOINT(finish.row, finish.col, rawdata);
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
      double dSRow = static_cast<double>(start.row);
      double dSCol = static_cast<double>(start.col);
      double dFRow = static_cast<double>(finish.row);
      double dFCol = static_cast<double>(finish.col);

      std::vector<std::pair<CG::route_point, CG::route_point>> cpPairVector;
      double k = (dSCol - dFCol) / (dSRow - dFRow);
      double b = dSCol - k * dSRow;
      double dDiameter = static_cast<double>(diameter);

      for (size_t lineIdx = 0; lineIdx < airRoutes.size() * iterations; lineIdx++)
      {
         double theta = atan(k);
         double theta_mod = lineIdx % 2 ? theta : -theta;
         // NOTE : при разлете в 45 грд.
         // NOTE : для смены угла разлета заменить на двойной синус
         double distantValue = sqrt(2)*(dDiameter * static_cast<double>((lineIdx + 1)/2));
         CG::geo_point startNonRotated{ dSRow + cos(theta)*distantValue, dSCol + sin(theta)*distantValue };
         // NOTE: разворот на 180, т.е. на Пи
         CG::geo_point finishNonRotated{ dFRow + cos(theta + PI) * distantValue, dFCol + sin(theta + PI) * distantValue };

         // NOTE: заюзана формула поворота относительно произвольной точки
         GeoMatrix rtResStart =
              GeoMatrix({ { startNonRotated.lat, startNonRotated.lon, 1. } })
            * GeoMatrix({ { 1., 0., 0. }
                        , { 0., 1., 0. }
                        , { -dSRow, -dSCol, 1. } })
            * GeoMatrix({ { cos(theta_mod), sin(theta_mod), 0. }
                        , { -sin(theta_mod), cos(theta_mod), 0. }
                        , { 0., 0., 1. } })
            * GeoMatrix({ { 1., 0., 0. }
                        , { 0., 1., 0. }
                        , { dSRow, dSCol, 1. } });
         GeoMatrix rtResFinish =
              GeoMatrix({ { finishNonRotated.lat, finishNonRotated.lon, 1. } })
            * GeoMatrix({ { 1., 0., 0. }
                        , { 0., 1., 0. }
                        , { -dFRow, -dFCol, 1. } })
            * GeoMatrix({ { cos(-theta_mod), sin(-theta_mod), 0. }
                        , { -sin(-theta_mod), cos(-theta_mod), 0. }
                        , { 0., 0., 1. } })
            * GeoMatrix({ { 1., 0., 0. }
                        , { 0., 1., 0. }
                        , { dFRow, dFCol, 1. } });
         cpPairVector.emplace_back(std::pair<CG::route_point, CG::route_point>{
              { static_cast<int>(rtResStart.Get(0, 0)), static_cast<int>(rtResStart.Get(0, 1)) }
            , { static_cast<int>(rtResFinish.Get(0, 0)), static_cast<int>(rtResFinish.Get(0, 1)) }
         });
      }

      // NOTE: если точки выходят за границу карты, система смещает их обратно, и при использовании облета не меняется покрытие, т.е. пути нет
      // TODO: переписать проходы, а то криво работает
      for (size_t lineIdx = 0; lineIdx < airRoutes.size()*iterations; lineIdx++)
      {
         size_t airIdx = lineIdx % airRoutes.size();
         //airRoutes.at(airIdx).controlPointList.clear();
         //controlPointInserter(airRoutes.at(airIdx), FLYCRCHCKPOINT(start.row, start.col, rawdata));
         int finishRow, finishCol, startRow, startCol;
         if (!(lineIdx / airRoutes.size()) % 2)
         {
            startRow = cpPairVector.at(lineIdx).first.row;
            startCol = cpPairVector.at(lineIdx).first.col;
            finishRow = cpPairVector.at(lineIdx).second.row;
            finishCol = cpPairVector.at(lineIdx).second.col;
         }
         else
         {
            startRow = cpPairVector.at(lineIdx).second.row;
            startCol = cpPairVector.at(lineIdx).second.col;
            finishRow = cpPairVector.at(lineIdx).first.row;
            finishCol = cpPairVector.at(lineIdx).first.col;
         }

         // NOTE: старт и финиш шагов итерации
         auto correctedStepStart = FLYCRCHCKPOINT(startRow, startCol, rawdata);
         auto correctedStepFinish = FLYCRCHCKPOINT(finishRow, finishCol, rawdata);
         controlPointInserter(airRoutes.at(airIdx), correctedStepStart);
         controlPointInserter(airRoutes.at(airIdx), correctedStepFinish);
      }
   }
   else
   {
      //ATLASSERT(false);
   }
   return true;
}

CG::route_point StrategyManager::checkRetranslateFlyPointAffilation(int row, int col, const std::shared_ptr<RoutePointMatrix>& rawdata)
{
   affilationCheckerMtd affilationChecker = [](const std::shared_ptr<pathfinder::RoutePointMatrix>& rawdata, size_t row, size_t col)->bool
   {
      return rawdata->Get(row, col).fly != pathfinder::FlyZoneAffilation::FZA_FORBIDDEN;
   };
   return CoordinateCorrectionHelper::CorrectPoint(rawdata, row, col, affilationChecker, GetCommunicator());
}