#include "stdafx.h"
#include "PathfindingEngine.h"
//#include "common/matrix.h"
//#include "main/dispatcher.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
//#include "settings/common/settings.h"
#include <windows.h>
#include "algorithm/statistic.h"

using namespace SV;
using namespace SV::engine;
std::recursive_mutex g_pathfindingEngineMutex;

PathfindingEngine::PathfindingEngine(central_pack* pack)
   : EngineBase(pack)
   , m_routedata(std::make_shared<pathfinder::route_data>())
{
   /*m_noGoLowLevel = 73.f;
   m_noGoHighLevel = 120.f;
   m_noFlyLevel = 139.f;
   m_minDangerAngle = 40.f;
   m_maxDangerAngle = 70.f;*/
}

void PathfindingEngine::ProcessPathFind(const pathfinder::path_finder_indata& scenarioData, const pathfinder::GeoMatrix& rawData, std::shared_ptr<settings::application_settings> settings, std::function<void(void)> completeCallback)
{
   m_settings = settings;
   std::thread(&PathfindingEngine::processPathFind, this, scenarioData, rawData, completeCallback).detach();
}

const pathfinder::UnsignedMatrix& PathfindingEngine::GetLandUnitExplication() const
{
   static pathfinder::UnsignedMatrix explication;
   size_t rowCount = m_rawdata.get()->GetRowCount();
   size_t colCount = m_rawdata.get()->GetColCount();
   explication.SetRowCount(rowCount);
   explication.SetColCount(colCount);
   for (size_t ridx = 0; ridx < rowCount; ridx++)
   {
      for (size_t cidx = 0; cidx < colCount; cidx++)
      {
         explication.Set(ridx, cidx, static_cast<size_t>(m_rawdata.get()->Get(ridx, cidx).go));
      }
   }
   return explication;
}

const pathfinder::UnsignedMatrix& PathfindingEngine::GetAirUnitExplication() const
{
   static pathfinder::UnsignedMatrix explication;
   size_t rowCount = m_rawdata.get()->GetRowCount();
   size_t colCount = m_rawdata.get()->GetColCount();
   explication.SetRowCount(rowCount);
   explication.SetColCount(colCount);
   for (size_t ridx = 0; ridx < rowCount; ridx++)
   {
      for (size_t cidx = 0; cidx < colCount; cidx++)
      {
         explication.Set(ridx, cidx, static_cast<size_t>(m_rawdata.get()->Get(ridx, cidx).fly));
      }
   }
   return explication;
}

void PathfindingEngine::processPathFind(const pathfinder::path_finder_indata& scenarioData, const pathfinder::GeoMatrix& rawData, std::function<void(void)> completeCallback)
{
   // TODO: разобраться с настройками
   convertMap(rawData, m_rawdata);
   processPathFindInternal(scenarioData, completeCallback);
}

void PathfindingEngine::convertMap(const pathfinder::GeoMatrix& rawdataSrc, std::shared_ptr<pathfinder::RoutePointMatrix> rawdataDst)
{
   size_t rowCount = rawdataSrc.GetRowCount();
   size_t colCount = rawdataSrc.GetColCount();
   rawdataDst->SetRowCount(rowCount);
   if (rowCount > 0)
      rawdataDst->SetColCount(colCount);
   for (size_t rIdx = 0; rIdx < rowCount; rIdx++)
   {
      for (size_t cIdx = 0; cIdx < colCount; cIdx++)
         rawdataDst->Set(rIdx, cIdx, CG::route_point(rIdx, cIdx, rawdataSrc.Get(rIdx, cIdx), checkFlyZone(rawdataSrc, rIdx, cIdx).fza, checkGoZone(rawdataSrc, rIdx, cIdx).gza));
   }
}

pathfinder::check_fly_zone_result PathfindingEngine::checkFlyZone(const pathfinder::GeoMatrix& rawdataSrc, int rowIdx, int colIdx)
{
   auto& pth_stt = m_settings->pth_stt;
   return { (rawdataSrc.Get(rowIdx, colIdx) > pth_stt.lvl_stt.max_air_height) ? pathfinder::FlyZoneAffilation::FZA_FORBIDDEN : pathfinder::FlyZoneAffilation::FZA_NORMAL };
}

pathfinder::check_go_zone_result PathfindingEngine::checkGoZone(const pathfinder::GeoMatrix& rawdataSrc, int rowIdx, int colIdx)
{
   // NOTE: стоблцы идут снизу вверх(по крайней мере тут считаем именно так)
   size_t rowCount = rawdataSrc.GetRowCount();
   size_t colCount = rawdataSrc.GetColCount();
   return checkAngles(
        rawdataSrc.Get(rowIdx, colIdx) // center
      , rawdataSrc.Get(rowIdx, colIdx > 0 ? colIdx - 1 : 0) // left
      , rawdataSrc.Get(rowIdx, colIdx < colCount - 1 ? colIdx + 1 : colCount - 1) // right
      , rawdataSrc.Get(rowIdx < rowCount - 1 ? rowIdx + 1 : rowCount - 1, colIdx) // top
      , rawdataSrc.Get(rowIdx > 0 ? rowIdx - 1 : 0, colIdx) // bottom
      , rawdataSrc.Get(rowIdx < rowCount - 1 ? rowIdx + 1 : rowCount - 1, colIdx > 0 ? colIdx - 1 : 0) // topleft
      , rawdataSrc.Get(rowIdx > 0 ? rowIdx - 1 : 0, colIdx > 0 ? colIdx - 1 : 0) // bottomleft
      , rawdataSrc.Get(rowIdx < rowCount - 1 ? rowIdx + 1 : rowCount - 1, colIdx < colCount - 1 ? colIdx + 1 : colCount - 1) // topright
      , rawdataSrc.Get(rowIdx > 0 ? rowIdx - 1 : 0, colIdx > 0 ? colIdx - 1 : 0) // bottomright
   );
}

pathfinder::check_go_zone_result PathfindingEngine::checkAngles(double center, double left, double right, double top, double bottom, double topleft, double bottomleft, double topright, double bottomright)
{
   auto& pth_stt = m_settings->pth_stt;
   // NOTE: проверяем 4 направления по 8 сторонам света
   pathfinder::check_go_zone_result result;
   double cellMult = 6.;
   float mult = 180.f/static_cast<float>(M_PI);
   // top-bottom
   float angleTB = (atan(fabs(top - center) / cellMult)*mult + atan(fabs(center - bottom) / cellMult)*mult)/2.;
   // left-right
   float angleLR = (atan(fabs(left - center) / cellMult)*mult + atan(fabs(center - right) / cellMult)*mult)/2.;
   // topleft-bottomright
   float angleTLBR = (atan(fabs(topleft - center) / cellMult) * mult + atan(fabs(center - bottomright) / cellMult) * mult) / 2.;
   // topright-bottomleft
   float angleTRBL = (atan(fabs(bottomleft - center) / cellMult) * mult + atan(fabs(center - topright) / cellMult) * mult) / 2.;
   result.asn = angleTB;
   result.awe = angleLR;
   result.aswne = angleTRBL;
   result.asenw = angleTLBR;

   auto minDA = pth_stt.lvl_stt.dangerous_land_angle;
   auto maxDA = pth_stt.lvl_stt.max_land_angle;
   auto minLH = pth_stt.lvl_stt.min_land_height;
   auto maxLH = pth_stt.lvl_stt.max_land_height;
   //qDebug() << "angles:" << fabs(angleUD) << fabs(angleLR);
   bool maxAngleExcess = (maxDA < fabs(angleTB)) || (maxDA < fabs(angleLR)) || (maxDA < fabs(angleTRBL)) || (maxDA < fabs(angleTLBR));
   bool minAngleExcess = (minDA < fabs(angleTB)) || (minDA < fabs(angleLR)) || (minDA < fabs(angleTRBL)) || (minDA < fabs(angleTLBR));
   if (maxAngleExcess
       || (center < minLH)
       || (center > maxLH))
      result.gza = pathfinder::GoZoneAffilation::GZA_FORBIDDEN;
   else if (minAngleExcess)
      result.gza = pathfinder::GoZoneAffilation::GZA_DANGEROUS;
   else
      result.gza = pathfinder::GoZoneAffilation::GZA_NORMAL;
   return result;
}

engine::iPathfindingEngine* CreatePathfindingEngine(central_pack* pack)
{
   return new engine::PathfindingEngine(pack);
}