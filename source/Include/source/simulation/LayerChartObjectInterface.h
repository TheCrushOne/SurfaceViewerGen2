#pragma once

#include "LayerEntityInterface.h"
#include "layer_provider/layer_chart_object.h"

namespace SV::surface_simulation
{
   struct iLayerChartObject : iLayerEntity
   {
      //! ������ ���������� �� �������� ��������������
      virtual const layer_provider::layer_chart_object* GetContourData() const = 0;

      virtual ~iLayerChartObject() = default;
   };
}