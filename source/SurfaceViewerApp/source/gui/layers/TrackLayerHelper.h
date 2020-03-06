#pragma once
#include "RenderLayers.h"
#include "colreg/ColregSimulation.h"

namespace TrackLayerHelper
{
   using RouteZoneWidthGetFunc = double(size_t routeIndex, bool left/*, colreg::ROUTE_ZONE_TYPE zone*/);
   void RenderShiptrack(render::iRender* renderer, const colreg::track_full_info_ref* track, const render::object_info& info, int trackLinesIntervalSeconds, bool renderXTE);
   void renderRoute(render::iRender* renderer, colreg::id_type id, const colreg::route_ref* route, const render::object_info& info, ColregSimulation::ROUTE_TYPE type);
   void renderRouteSegment(render::iRender* renderer, colreg::id_type id, size_t index, const colreg::route_point& rp1, const colreg::route_point& rp2, const render::object_info& info, ColregSimulation::ROUTE_TYPE type);
   void renderRouteZone(render::iRender* renderer, const std::function<RouteZoneWidthGetFunc>& func, const colreg::route_ref* route, COLORREF color);
}