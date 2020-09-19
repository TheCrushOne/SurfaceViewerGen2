#include "stdafx.h"
#include "WaveFrontline.h"
#include "common\pathfinder_structs.h"
#include "SVCG/positioning.h"
#include "AlgorithmHelpers.h"

using namespace chart_object;
std::recursive_mutex g_waveFrontlineMutex;

using frontlineElement = std::pair<SVCG::route_point, size_t>;
using frontlineType = std::vector<frontlineElement>;
using lineVct = std::vector<std::vector<SVCG::route_point>>;

// WARNING: Не натравливать на пустой парент!!! падает!!!
inline size_t addDuplicateIsolineShard(lineVct& shardVct, std::vector<SVCG::route_point> parent)
{
   shardVct.emplace_back();
   auto lineIter = shardVct.end() - 1;
   // Базовая линия дублируется на каждую новую
   lineIter->insert(lineIter->end(), parent.begin(), parent.end());
   return shardVct.size() - 1;
}

inline frontlineType checkAddSurrondPoints(const std::shared_ptr<pathfinder::Matrix<bool>>& actValMtx, const std::shared_ptr<pathfinder::Matrix<bool>>& inLineFlagMtx, const std::shared_ptr<pathfinder::Matrix<bool>>& passedFlagMtx, lineVct& shardVct, size_t parentIdx, size_t rIdx, size_t cIdx)
{
   frontlineType result;
   std::vector<SVCG::route_point> surr;
   int row = static_cast<int>(rIdx);
   int col = static_cast<int>(cIdx);

   surr.emplace_back(SVCG::route_point{ row - 1, col - 1 });
   surr.emplace_back(SVCG::route_point{ row - 1, col });
   surr.emplace_back(SVCG::route_point{ row - 1, col + 1 });
   surr.emplace_back(SVCG::route_point{ row, col - 1 });
   surr.emplace_back(SVCG::route_point{ row, col + 1 });
   surr.emplace_back(SVCG::route_point{ row + 1, col - 1 });
   surr.emplace_back(SVCG::route_point{ row + 1, col });
   surr.emplace_back(SVCG::route_point{ row + 1, col + 1 });

   std::vector<SVCG::route_point> parentData = shardVct.at(parentIdx);

   bool first = true;
   for (auto& elem : surr)
   {
      int row = elem.row, col = elem.col;
      if ((activationCheck(actValMtx, row, col))
         && (inLineCheck(inLineFlagMtx, row, col))
         && (!passedCheck(passedFlagMtx, row, col)))
      {
         passedFlagMtx->Set(row, col, true);
         size_t lineIdx;
         if (first)
         {
            first = false;
            lineIdx = parentIdx;
         }
         else
            lineIdx = addDuplicateIsolineShard(shardVct, parentData);
         shardVct.at(lineIdx).emplace_back(SVCG::route_point(row, col));
         result.push_back(frontlineElement{ SVCG::route_point(row, col), lineIdx });
      }
   }
   return result;
}

std::vector<geometry_chart_object> WaveFrontline::generateIsolineLevel(const pathfinder::GeoMatrix* rawdata, double height, int H)
{
   if (!rawdata->GetRowCount())
      return std::vector<geometry_chart_object>();

   auto actValMtx = std::make_shared<pathfinder::Matrix<bool>>(rawdata->GetRowCount(), rawdata->GetColCount(), false);
   auto inLineFlagMtx = std::make_shared<pathfinder::Matrix<bool>>(rawdata->GetRowCount(), rawdata->GetColCount(), false);
   auto passedFlagMtx = std::make_shared<pathfinder::Matrix<bool>>(rawdata->GetRowCount(), rawdata->GetColCount(), false);

   for (size_t rIdx = 0; rIdx < rawdata->GetRowCount(); rIdx++)
   {
      for (size_t cIdx = 0; cIdx < rawdata->GetColCount(); cIdx++)
      {
         if (rawdata->Get(rIdx, cIdx) >= height)
            actValMtx->Set(rIdx, cIdx, true);
      }
   }

   for (size_t rIdx = 0; rIdx < rawdata->GetRowCount(); rIdx++)
   {
      for (size_t cIdx = 0; cIdx < rawdata->GetColCount(); cIdx++)
         inLineFlagMtx->Set(rIdx, cIdx, activationCheck(actValMtx, rIdx, cIdx) && nearestActivationCheck(actValMtx, rIdx, cIdx));
   }

   lineVct isoLineVct;
   // Построение контура
   for (size_t rIdx = 0; rIdx < actValMtx->GetRowCount(); rIdx++)
   {
      for (size_t cIdx = 0; cIdx < actValMtx->GetColCount(); cIdx++)
      {
         int row = static_cast<int>(rIdx);
         int col = static_cast<int>(cIdx);
         // С этой точки начинаем обход в ширину
         if (activationCheck(actValMtx, row, col) && inLineCheck(inLineFlagMtx, row, col) && !passedCheck(passedFlagMtx, row, col))
         {
            SVCG::route_point root{row, col};
            frontlineType frontline;
            lineVct shardVct;
            shardVct.emplace_back();
            frontline.emplace_back(root, shardVct.size() - 1);
            shardVct.begin()->emplace_back(root);
            // Обход изолинии в ширь
            while (!frontline.empty())
            {
               frontlineType new_frontline;
               for (auto& elem : frontline)
               {
                  auto checked = checkAddSurrondPoints(actValMtx, inLineFlagMtx, passedFlagMtx, shardVct, elem.second, elem.first.row, elem.first.col);
                  new_frontline.insert(new_frontline.end(), checked.begin(), checked.end());
               }
               frontline = new_frontline;
            }

            // Как бы мерж
            isoLineVct.insert(isoLineVct.end(), shardVct.begin(), shardVct.end());
         }
      }
   }

   settings::environment_settings& env_stt = GetService()->GetSettingsSerializerHolder()->GetSettings().env_stt;
   std::vector<geometry_chart_object> res;
   auto& gcBack = res.emplace_back();
   for (auto& line : isoLineVct)
   {
      auto& cBack = gcBack.geom_contour_vct.emplace_back();
      for (auto& point : line)
         cBack.emplace_back(static_cast<math::geo_point>(SVCG::RoutePointToPositionPoint(point, env_stt)));
   }

   std::lock_guard<std::recursive_mutex> guard(g_waveFrontlineMutex);
   return res;
}