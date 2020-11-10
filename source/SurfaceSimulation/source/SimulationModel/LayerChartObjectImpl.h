#pragma once
#include "simulation/LayerChartObjectInterface.h"

namespace SV::surface_simulation
{
   class LayerChartObjectImpl
      : public iLayerChartObject
   {
   public:
      LayerChartObjectImpl()
      {}

      //layer_provider::ship_info GetInfo() const override final { return m_info; }
      const CG::geo_contour_vct& GetContourData() const override final { return m_chartObject.geom_contour_vct; }
      const chart_object_id GetId() const override final { return m_chartObject.id; }
      const chart_object::OBJECT_TYPE GetType() const override final { return m_chartObject.type; }

      const properties::simple_prop_vct* GetProps() const override final { return &m_chartObject.prop_vct; }

      void SetSrcRoute(const SV::layer_provider::layer_chart_object& src) { m_chartObject = src; }
   private:
      SV::layer_provider::layer_chart_object m_chartObject;
   };
}