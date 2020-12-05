#pragma once
#include "RenderLayers.h"
#include "layer_provider/trajectory_point.h"
#include "colreg/SimulationTypes.h"

namespace SV::TrackLayerHelper
{
   using RouteZoneWidthGetFunc = double(size_t routeIndex, bool left/*, colreg::ROUTE_ZONE_TYPE zone*/);
   using PointVisibilityChecker = std::function<bool(size_t, const CG::geo_point&)>;
   using SegmentVisibilityChecker = std::function<bool(size_t)>;
   void RenderShiptrack(render::iRender* renderer, const CG::layer_provider::trajectory_point_vct* track, const render::object_info& info, int trackLinesIntervalSeconds, bool renderXTE);
   void renderRoute(render::iRender* renderer, id_type id, const CG::layer_provider::trajectory_point_vct& route, const render::object_info& info, surface_simulation::ROUTE_TYPE type, SegmentVisibilityChecker segVisChecker, PointVisibilityChecker pntVisChecker);
   void renderRouteSegment(render::iRender* renderer, id_type id, size_t index, const CG::layer_provider::trajectory_point& rp1, const CG::layer_provider::trajectory_point& rp2, const render::object_info& info, surface_simulation::ROUTE_TYPE type);
   void renderRouteZone(render::iRender* renderer, const std::function<RouteZoneWidthGetFunc>& func, const CG::layer_provider::trajectory_point_vct* route, COLORREF color);
}