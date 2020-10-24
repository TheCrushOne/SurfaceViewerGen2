#pragma once
#include "RenderLayers.h"
#include "colreg/ColregSimulation.h"
#include "simulation\LayerUnitInterface.h"

namespace SV
{
   void RenderUnitContour(render::iRender* renderer, surface_simulation::UNIT_TYPE type, const layer_provider::ship_info& ship_info, const CG::layer_provider::trajectory_point& center, const render::object_info& info);
   void RenderDomain(render::FIND_OBJECT_TYPE fot, render::iRender* renderer, const surface_simulation::iLayerUnit* unit, const CG::layer_provider::trajectory_point& center, double timeFromNow, const render::object_info& info/*, const colreg::domain_scales* scales = 0*/);
   void RenderArrow(render::iRender* renderer, const CG::geo_point& pTo, double direction, const render::object_info& info, double sizeKoef = 1.0);
}