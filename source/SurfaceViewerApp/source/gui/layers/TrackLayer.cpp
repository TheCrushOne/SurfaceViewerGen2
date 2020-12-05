#include "stdafx.h"
#include "TrackLayer.h"
#include "simulator/simulator.h"
#include "TrackLayerHelper.h"
#include "SVCG/positioning.h"

using namespace SV;

namespace
{
   TrackLayer layer;
   const render::object_info rover_route_obj_info()
   {
      return {
         1,
         render::LINE_STYLE::LL_DASH,
         render::FILL_TYPE::FT_NONE,
         RGB(110, 110, 110)
      };
   };

   const render::object_info rover_control_points_obj_info()
   {
      return {
         1,
         render::LINE_STYLE::LL_NONE,
         render::FILL_TYPE::FT_NONE,
         RGB(150, 150, 150)
      };
   };

   const render::object_info drone_route_obj_info()
   {
      return {
         1,
         render::LINE_STYLE::LL_DASH,
         render::FILL_TYPE::FT_NONE,
         RGB(255, 200, 30)
      };
   };

   const render::object_info drone_control_points_obj_info()
   {
      return {
         1,
         render::LINE_STYLE::LL_NONE,
         render::FILL_TYPE::FT_NONE,
         RGB(255, 255, 30)
      };
   };
}

bool TrackLayer::onScenarioPathFound()
{
   return onAnyPathFound();
}

bool TrackLayer::onScenarioOptPathFound()
{
   return onAnyPathFound();
}

bool TrackLayer::onAnyPathFound()
{
   auto* sim = simulator::getSimulator();
   if (!sim || sim->GetSimulatorScenarioState() == surface_simulation::SCENARIO_STATUS::SS_NOT_LOADED)
      return false;
   const auto& simulationState = sim->GetState();

   for (size_t idx = 0; idx < simulationState.GetUnitCount(surface_simulation::UNIT_TYPE::UT_ROVER); ++idx)
   {
      const auto* rover = simulationState.GetUnitByIdx(surface_simulation::UNIT_TYPE::UT_ROVER, idx);
      ATLASSERT(rover);

      auto rover_id = rover->GetInfo().id;

      auto& route = rover->GetSrcPath();
      for (size_t idx = 0; idx < route.size(); idx++)
      {
         if (idx < route.size() - 1)
            sim->AddLayerVisibilityInfoUnit(formatIdRoverSrcRouteSegmentsIdxPath(rover_id, idx), m_renderSourceRoute);
         sim->AddLayerVisibilityInfoUnit(formatIdRoverSrcRoutePointsIdxPath(rover_id, idx, transfercase::PositionPointToRoutePoint(route.at(idx).pos, sim->GetAppSettings().env_stt)), m_renderSourceRoute);
      }

      auto& cps = rover->GetSrcControlPoints();
      for (size_t idx = 0; idx < cps.size(); idx++)
      {
         if (idx < cps.size() - 1)
            sim->AddLayerVisibilityInfoUnit(formatIdRoverSrcControlPointsSegmentsIdxPath(rover_id, idx), m_renderControlPoints);
         sim->AddLayerVisibilityInfoUnit(formatIdRoverSrcControlPointsPointsIdxPath(rover_id, idx), m_renderControlPoints);
      }
   }

   for (size_t idx = 0; idx < simulationState.GetUnitCount(surface_simulation::UNIT_TYPE::UT_DRONE); ++idx)
   {
      const auto* drone = simulationState.GetUnitByIdx(surface_simulation::UNIT_TYPE::UT_DRONE, idx);
      ATLASSERT(drone);

      auto drone_id = drone->GetInfo().id;

      auto& route = drone->GetSrcPath();
      for (size_t idx = 0; idx < route.size(); idx++)
      {
         sim->AddLayerVisibilityInfoUnit(formatIdDroneSrcRouteSegmentsIdxPath(drone_id, idx), m_renderSourceRoute);
         sim->AddLayerVisibilityInfoUnit(formatIdDroneSrcRoutePointsIdxPath(drone_id, idx), m_renderSourceRoute);
      }
      sim->AddLayerVisibilityInfoUnit(formatIdDroneSrcRoutePointsIdxPath(drone_id, route.size()), m_renderSourceRoute);

      auto& cps = drone->GetSrcControlPoints();
      for (size_t idx = 0; idx < cps.size(); idx++)
      {
         sim->AddLayerVisibilityInfoUnit(formatIdDroneSrcControlPointsSegmentsIdxPath(drone_id, idx), m_renderControlPoints);
         sim->AddLayerVisibilityInfoUnit(formatIdDroneSrcControlPointsPointsIdxPath(drone_id, idx), m_renderControlPoints);
      }
      sim->AddLayerVisibilityInfoUnit(formatIdDroneSrcControlPointsPointsIdxPath(drone_id, cps.size()), m_renderControlPoints);
   }
}

void TrackLayer::renderTracks(render::iRender* renderer)
{
   const auto* sim = simulator::getSimulator();
   if (!sim || sim->GetSimulatorScenarioState() == surface_simulation::SCENARIO_STATUS::SS_NOT_LOADED)
      return;
   const auto& simulationState = sim->GetState();

   for (size_t idx = 0; idx < simulationState.GetUnitCount(surface_simulation::UNIT_TYPE::UT_ROVER); ++idx)
   {
      const auto* rover = simulationState.GetUnitByIdx(surface_simulation::UNIT_TYPE::UT_ROVER, idx);
      ATLASSERT(rover);

      auto rover_id = rover->GetInfo().id;

      if (!LayerFiltersManager::GetInstance().IsFilterVisible(formatIdRoverPath(rover_id)))
         continue;

      if (m_renderSourceRoute
         && LayerFiltersManager::GetInstance().IsFilterVisible(formatIdRoverSrcRoutePath(rover_id)))
      {
         TrackLayerHelper::SegmentVisibilityChecker segmentVisibilityCheck = [rover_id, this](size_t idx)->bool {
            return LayerFiltersManager::GetInstance().IsFilterVisible(formatIdRoverSrcRouteSegmentsIdxPath(rover_id, idx));
         };
         auto& env_stt = sim->GetAppSettings().env_stt;
         TrackLayerHelper::PointVisibilityChecker pointVisibilityCheck = [rover_id, env_stt, this](size_t idx, const CG::geo_point& pnt)->bool {
            return LayerFiltersManager::GetInstance().IsFilterVisible(formatIdRoverSrcRoutePointsIdxPath(rover_id, idx, transfercase::PositionPointToRoutePoint(pnt, env_stt)));
         };
         TrackLayerHelper::renderRoute(renderer, rover_id, rover->GetSrcPath(), rover_route_obj_info(), surface_simulation::ROUTE_TYPE::RT_SOURSE, segmentVisibilityCheck, pointVisibilityCheck);
      }

      if (m_renderControlPoints
         && LayerFiltersManager::GetInstance().IsFilterVisible(formatIdRoverSrcControlPointsPath(rover_id)))
      {
         TrackLayerHelper::SegmentVisibilityChecker segmentVisibilityCheck = [rover_id, this](size_t idx)->bool {
            return LayerFiltersManager::GetInstance().IsFilterVisible(formatIdRoverSrcControlPointsSegmentsIdxPath(rover_id, idx));
         };
         auto& env_stt = sim->GetAppSettings().env_stt;
         TrackLayerHelper::PointVisibilityChecker pointVisibilityCheck = [rover_id, env_stt, this](size_t idx, const CG::geo_point& pnt)->bool {
            return LayerFiltersManager::GetInstance().IsFilterVisible(formatIdRoverSrcControlPointsPointsIdxPath(rover_id, idx));
         };
         TrackLayerHelper::renderRoute(renderer, rover_id, rover->GetSrcControlPoints(), rover_control_points_obj_info(), surface_simulation::ROUTE_TYPE::RT_CONTROL, segmentVisibilityCheck, pointVisibilityCheck);
      }
   }

   for (size_t idx = 0; idx < simulationState.GetUnitCount(surface_simulation::UNIT_TYPE::UT_DRONE); ++idx)
   {
      const auto* drone = simulationState.GetUnitByIdx(surface_simulation::UNIT_TYPE::UT_DRONE, idx);
      ATLASSERT(drone);

      auto drone_id = drone->GetInfo().id;

      if (!LayerFiltersManager::GetInstance().IsFilterVisible(formatIdDronePath(drone_id)))
         continue;

      if (m_renderSourceRoute
         && LayerFiltersManager::GetInstance().IsFilterVisible(formatIdDroneSrcRoutePath(drone_id)))
      {
         TrackLayerHelper::SegmentVisibilityChecker segmentVisibilityCheck = [drone_id, this](size_t idx)->bool {
            return LayerFiltersManager::GetInstance().IsFilterVisible(formatIdDroneSrcRouteSegmentsIdxPath(drone_id, idx));
         };
         auto& env_stt = sim->GetAppSettings().env_stt;
         TrackLayerHelper::PointVisibilityChecker pointVisibilityCheck = [drone_id, env_stt, this](size_t idx, const CG::geo_point& pnt)->bool {
            return LayerFiltersManager::GetInstance().IsFilterVisible(formatIdDroneSrcRoutePointsIdxPath(drone_id, idx));
         };
         TrackLayerHelper::renderRoute(renderer, drone->GetInfo().id, drone->GetSrcPath(), drone_route_obj_info(), surface_simulation::ROUTE_TYPE::RT_SOURSE, segmentVisibilityCheck, pointVisibilityCheck);
      }

      if (m_renderControlPoints
         && LayerFiltersManager::GetInstance().IsFilterVisible(formatIdDroneSrcControlPointsPath(drone_id)))
      {
         TrackLayerHelper::SegmentVisibilityChecker segmentVisibilityCheck = [drone_id, this](size_t idx)->bool {
            return LayerFiltersManager::GetInstance().IsFilterVisible(formatIdDroneSrcControlPointsSegmentsIdxPath(drone_id, idx));
         };
         auto& env_stt = sim->GetAppSettings().env_stt;
         TrackLayerHelper::PointVisibilityChecker pointVisibilityCheck = [drone_id, env_stt, this](size_t idx, const CG::geo_point& pnt)->bool {
            return LayerFiltersManager::GetInstance().IsFilterVisible(formatIdDroneSrcControlPointsPointsIdxPath(drone_id, idx));
         };
         TrackLayerHelper::renderRoute(renderer, drone->GetInfo().id, drone->GetSrcControlPoints(), drone_control_points_obj_info(), surface_simulation::ROUTE_TYPE::RT_CONTROL, segmentVisibilityCheck, pointVisibilityCheck);
      }
   }
}

layer_filter_path TrackLayer::formatIdDronePath(SV::id_type id)
{
   return layer_filter_path{ layer_filter_tag::drones, layer_filter_tag::drone_templ + std::to_string(id) };
}

layer_filter_path TrackLayer::formatIdDroneSrcRoutePath(SV::id_type id)
{
   auto res = formatIdDronePath(id);
   res.emplace_back(layer_filter_tag::source_route);
   return res;
}

layer_filter_path TrackLayer::formatIdDroneSrcRouteSegmentsPath(SV::id_type id)
{
   auto res = formatIdDroneSrcRoutePath(id);
   res.emplace_back(layer_filter_tag::segments);
   return res;
}

layer_filter_path TrackLayer::formatIdDroneSrcRouteSegmentsIdxPath(SV::id_type id, size_t idx)
{
   auto res = formatIdDroneSrcRouteSegmentsPath(id);
   res.emplace_back(layer_filter_tag::segment_templ + std::to_string(idx) + "->" + std::to_string(idx + 1));
   return res;
}

layer_filter_path TrackLayer::formatIdDroneSrcRoutePointsPath(SV::id_type id)
{
   auto res = formatIdDroneSrcRoutePath(id);
   res.emplace_back(layer_filter_tag::points);
   return res;
}

layer_filter_path TrackLayer::formatIdDroneSrcRoutePointsIdxPath(SV::id_type id, size_t idx, CG::route_point point)
{
   std::ostringstream out;
   out.precision(2);
   out << std::fixed << "[" << point.row << ", " << point.col << "]";
   auto res = formatIdDroneSrcRoutePointsPath(id);
   res.emplace_back(layer_filter_tag::point_templ + std::to_string(idx) + out.str());
   return res;
}

layer_filter_path TrackLayer::formatIdDroneSrcControlPointsPath(SV::id_type id)
{
   auto res = formatIdDronePath(id);
   res.emplace_back(layer_filter_tag::control_points);
   return res;
}

layer_filter_path TrackLayer::formatIdDroneSrcControlPointsSegmentsPath(SV::id_type id)
{
   auto res = formatIdDroneSrcControlPointsPath(id);
   res.emplace_back(layer_filter_tag::segments);
   return res;
}

layer_filter_path TrackLayer::formatIdDroneSrcControlPointsSegmentsIdxPath(SV::id_type id, size_t idx)
{
   auto res = formatIdDroneSrcControlPointsSegmentsPath(id);
   res.emplace_back(layer_filter_tag::segment_templ + std::to_string(idx) + "->" + std::to_string(idx + 1));
   return res;
}

layer_filter_path TrackLayer::formatIdDroneSrcControlPointsPointsPath(SV::id_type id)
{
   auto res = formatIdDroneSrcControlPointsPath(id);
   res.emplace_back(layer_filter_tag::points);
   return res;
}

layer_filter_path TrackLayer::formatIdDroneSrcControlPointsPointsIdxPath(SV::id_type id, size_t idx, CG::route_point point)
{
   std::ostringstream out;
   out.precision(2);
   out << std::fixed << "[" << point.row << ", " << point.col << "]";
   auto res = formatIdDroneSrcControlPointsPointsPath(id);
   res.emplace_back(layer_filter_tag::point_templ + std::to_string(idx) + out.str());
   return res;
}

layer_filter_path TrackLayer::formatIdRoverPath(SV::id_type id)
{
   return layer_filter_path{ layer_filter_tag::rovers, layer_filter_tag::rover_templ + std::to_string(id) };
}

layer_filter_path TrackLayer::formatIdRoverSrcRoutePath(SV::id_type id)
{
   auto res = formatIdRoverPath(id);
   res.emplace_back(layer_filter_tag::source_route);
   return res;
}

layer_filter_path TrackLayer::formatIdRoverSrcRouteSegmentsPath(SV::id_type id)
{
   auto res = formatIdRoverSrcRoutePath(id);
   res.emplace_back(layer_filter_tag::segments);
   return res;
}

layer_filter_path TrackLayer::formatIdRoverSrcRouteSegmentsIdxPath(SV::id_type id, size_t idx)
{
   auto res = formatIdRoverSrcRouteSegmentsPath(id);
   res.emplace_back(layer_filter_tag::segment_templ + std::to_string(idx) + "->" + std::to_string(idx + 1));
   return res;
}

layer_filter_path TrackLayer::formatIdRoverSrcRoutePointsPath(SV::id_type id)
{
   auto res = formatIdRoverSrcRoutePath(id);
   res.emplace_back(layer_filter_tag::points);
   return res;
}

layer_filter_path TrackLayer::formatIdRoverSrcRoutePointsIdxPath(SV::id_type id, size_t idx, CG::route_point point)
{
   std::ostringstream out;
   out.precision(2);
   out << std::fixed << "[" << point.row << ", " << point.col << "]";
   auto res = formatIdRoverSrcRoutePointsPath(id);
   res.emplace_back(layer_filter_tag::point_templ + std::to_string(idx) + out.str());
   return res;
}

layer_filter_path TrackLayer::formatIdRoverSrcControlPointsPath(SV::id_type id)
{
   auto res = formatIdRoverPath(id);
   res.emplace_back(layer_filter_tag::control_points);
   return res;
}

layer_filter_path TrackLayer::formatIdRoverSrcControlPointsSegmentsPath(SV::id_type id)
{
   auto res = formatIdRoverSrcControlPointsPath(id);
   res.emplace_back(layer_filter_tag::segments);
   return res;
}

layer_filter_path TrackLayer::formatIdRoverSrcControlPointsSegmentsIdxPath(SV::id_type id, size_t idx)
{
   auto res = formatIdRoverSrcControlPointsSegmentsPath(id);
   res.emplace_back(layer_filter_tag::segment_templ + std::to_string(idx) + "->" + std::to_string(idx + 1));
   return res;
}

layer_filter_path TrackLayer::formatIdRoverSrcControlPointsPointsPath(SV::id_type id)
{
   auto res = formatIdRoverSrcControlPointsPath(id);
   res.emplace_back(layer_filter_tag::points);
   return res;
}

layer_filter_path TrackLayer::formatIdRoverSrcControlPointsPointsIdxPath(SV::id_type id, size_t idx, CG::route_point point)
{
   std::ostringstream out;
   out.precision(2);
   out << std::fixed << "[" << point.row << ", " << point.col << "]";
   auto res = formatIdRoverSrcControlPointsPointsPath(id);
   res.emplace_back(layer_filter_tag::point_templ + std::to_string(idx) + out.str());
   return res;
}