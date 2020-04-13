#pragma once

#include "RenderLayers.h"
#include "scenario\ScenarioManager.h"

class ChartLayer
   : public LayersContainer
   , public ScenarioObserverBase
{
public:
   ChartLayer() { initObjInfo(); }

   void Render(render::iRender* renderer) override;
   bool OnScenarioLoad() override { m_chartUSN = colreg::INVALID_ID; return true; }
   bool OnScenarioModified() { m_chartUSN = colreg::INVALID_ID; return true; }

   iProperty* GetProperties() override;

private:
   void initObjInfo();
   bool synchronize_map(/*const colreg::iChartSafetyCheck* checker, */render::iRender* renderer, const colreg::chart_objects_ref& chartObjects);
   void addChartObject(render::iRender* renderer, const colreg::chart_object& obj);
   void onLayerEnabledChanged() override;
   bool isLayerEnabled() const override { return true; }
private:

   std::unordered_map< colreg::OBJECT_TYPE, render::object_info> m_objInfo;

   iPropertyPtr   _props;

   bool  _showDepthChartObjects = true;
   iPropertyPtr   _prop_showDepthChartObjects;
   void OnShowDepthChartObjectsChanged();

   float _minScale2ShowDepthObjects = 50.;
   iPropertyPtr   _prop_minScale2ShowDepthObjects;
   size_t         m_chartUSN = colreg::INVALID_ID;
};