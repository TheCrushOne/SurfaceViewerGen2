#pragma once
#include "simulation/LayerChartObjectInterface.h"

namespace SV::surface_simulation
{
   class LayerChartObjectImpl : public iLayerChartObject
   {
   public:
      LayerChartObjectImpl()
      {}

      //layer_provider::ship_info GetInfo() const override final { return m_info; }
      const layer_provider::layer_chart_object* GetContourData() const override final { return &m_chartObject; }
      properties::simple_prop_vct* GetProps() const override final {  }

      void SetSrcRoute(const SV::layer_provider::layer_chart_object& src) { m_chartObject = src; }
   private:
      SV::layer_provider::layer_chart_object m_chartObject;
   };
}