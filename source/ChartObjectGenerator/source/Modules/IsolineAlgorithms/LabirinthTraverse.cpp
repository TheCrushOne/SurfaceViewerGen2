#include "stdafx.h"
#include "LabirinthTraverse.h"
#include "common\pathfinder_structs.h"
#include "SVCG/positioning.h"
#include "AlgorithmHelpers.h"

using namespace chart_object;
std::recursive_mutex g_labirinthTraverseMutex;

inline bool pointCheck(const std::shared_ptr<pathfinder::Matrix<bool>>& actValMtx, const std::shared_ptr<pathfinder::Matrix<bool>>& inLineFlagMtx, const std::shared_ptr<pathfinder::Matrix<bool>>& passedFlagMtx, SVCG::route_point& res, TraversalDirection dir, size_t rIdx, size_t cIdx)
{
   int toff = static_cast<int>(rIdx) + traversalOffset.at(dir).first.first;
   int tofs = static_cast<int>(cIdx) + traversalOffset.at(dir).first.second;
   int tosf = static_cast<int>(rIdx) + traversalOffset.at(dir).second.first;
   int toss = static_cast<int>(cIdx) + traversalOffset.at(dir).second.second;

   if ((activationCheck(actValMtx, toff, tofs))
      && (inLineCheck(inLineFlagMtx, toff, tofs))
      && (!passedCheck(passedFlagMtx, toff, tofs)))
   {
      res = { toff, tofs };
      return true;
   }
   if ((activationCheck(actValMtx, tosf, toss))
      && (inLineCheck(inLineFlagMtx, tosf, toss))
      && (!passedCheck(passedFlagMtx, tosf, toss)))
   {
      res = { tosf, toss };
      return true;
   }
   return false;
}

void LabirinthTraverse::GenerateIsolineLevel(const pathfinder::GeoMatrix* rawdata, double height, int H)
{
   auto rawRowCount = rawdata->GetRowCount(), rawColCount = rawdata->GetColCount();
   // TODO: установить минимальные ограничения
   if (!rawRowCount || !rawColCount)
      return;
   auto actValMtx = std::make_shared<pathfinder::Matrix<bool>>(rawRowCount, rawColCount, false);
   auto inLineFlagMtx = std::make_shared<pathfinder::Matrix<bool>>(rawRowCount, rawColCount, false);
   auto passedFlagMtx = std::make_shared<pathfinder::Matrix<bool>>(rawRowCount, rawColCount, false);

   for (size_t rIdx = 0; rIdx < rawRowCount; rIdx++)
   {
      for (size_t cIdx = 0; cIdx < rawColCount; cIdx++)
      {
         if (rawdata->Get(rIdx, cIdx) >= height)
            actValMtx->Set(rIdx, cIdx, true);
      }
   }

   for (size_t rIdx = 0; rIdx < rawRowCount; rIdx++)
   {
      for (size_t cIdx = 0; cIdx < rawColCount; cIdx++)
         inLineFlagMtx->Set(rIdx, cIdx, activationCheck(actValMtx, rIdx, cIdx) && nearestActivationCheck(actValMtx, rIdx, cIdx));
   }

   auto checkFindNextPoint = [&actValMtx, &inLineFlagMtx, &passedFlagMtx](size_t& dirChanges, SVCG::route_point& candidate, TraversalDirection& dir, SVCG::route_point current)
   {
      SVCG::route_point subcandidate;
      dir = notFoundTransitionRule.at(dir);
      if (!pointCheck(actValMtx, inLineFlagMtx, passedFlagMtx, subcandidate, dir, current.row, current.col))
      {
         while (!pointCheck(actValMtx, inLineFlagMtx, passedFlagMtx, subcandidate, dir, current.row, current.col) && dirChanges < 4)
         {
            dir = nextStepTransitionRule.at(dir);
            dirChanges++;
         }
      }
      candidate = subcandidate;
   };

   std::vector<std::vector<SVCG::route_point>> isoLineVct;
   // Построение контура
   for (size_t rIdx = 0; rIdx < actValMtx->GetRowCount(); rIdx++)
   {
      for (size_t cIdx = 0; cIdx < actValMtx->GetColCount(); cIdx++)
      {
         if (activationCheck(actValMtx, rIdx, cIdx) && inLineCheck(inLineFlagMtx, rIdx, cIdx) && !passedCheck(passedFlagMtx, rIdx, cIdx))
         {
            TraversalDirection dir = TraversalDirection::TD_R;
            std::vector<SVCG::route_point> resultList;
            SVCG::route_point start{ static_cast<int>(rIdx), static_cast<int>(cIdx) }, cur;
            resultList.push_back(start);
            cur = start;
            size_t dirChanges = 0;
            SVCG::route_point candidate;
            checkFindNextPoint(dirChanges, candidate, dir, cur);
            if (dirChanges >= 4)
               break;
            cur = candidate;
            //passedFlagMtx->Set(cur.row, cur.col, true);
            while (cur.row != start.row || cur.col != start.col)
            {
               //dir = TraversalDirection::TD_R;
               dirChanges = 0;
               checkFindNextPoint(dirChanges, candidate, dir, cur);
               ATLASSERT(dirChanges < 4);
               resultList.push_back(candidate);
               cur = candidate;
               //passedFlagMtx->Set(cur.row, cur.col, true);
            }
            // Метим элементы текущей изолинии как пройденные
            for (auto& elem : resultList)
               passedFlagMtx->Set(elem.row, elem.col, true);
            isoLineVct.emplace_back(resultList);
         }
      }
   }

   ATLASSERT(false); // TODO: убрать, когда появится норм проброс настроек
   settings::environment_settings envstt;// = GetPack()->settings->env_stt;
   std::vector<math::geo_points> isoLineGeoVct;
   for (auto& line : isoLineVct)
   {
      math::geo_points geoline;
      for (auto& point : line)
         geoline.emplace_back(SVCG::RoutePointToPositionPoint(point, envstt));
      isoLineGeoVct.emplace_back(geoline);
   }

   std::lock_guard<std::recursive_mutex> guard(g_labirinthTraverseMutex);
   m_chartObjectSetAdder(isoLineGeoVct, height, H);
}