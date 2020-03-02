#pragma once
#include "RenderLayers.h"
//#include "..\ColregTestApp\Colreg\Simulator.h"

//void RenderShipContour(render::iRender* renderer, const colreg::ship_info& ship_info, const colreg::track_point_info& center, const render::object_info& info);
//void RenderDomain(render::iRender* renderer, const ColregSimulation::iShip& ship, const colreg::track_point_info& center, double timeFromNow, const render::object_info& info, const colreg::domain_scales* scales = 0);
void RenderArrow(render::iRender* renderer, const geo_point& pTo, double direction, const render::object_info& info, double sizeKoef = 1.0);