#pragma once

#include "LayerEntityInterface.h"
#include "layer_provider/layer_chart_object.h"

namespace SV::surface_simulation
{
   struct iLayerChartObject
      : iLayerEntity
   {
      //! Полная информация по текущему местоположению
      virtual const CG::geo_contour_vct& GetContourData() const = 0;
      virtual const chart_object_id GetId() const = 0;
      virtual const chart_object::OBJECT_TYPE GetType() const = 0;

      virtual ~iLayerChartObject() = default;
   };
}