#pragma once

#include "RenderLayers.h"
#include "layer_provider\layer_chart_object.h"
#include "common\chart_object_unit.h"
#include "scenario\ScenarioManager.h"

namespace SV
{
   class ChartLayer
      : public LayersContainer
      , public ScenarioObserverBase
   {
   public:
      ChartLayer() { initObjInfo(); }

      void Render(render::iRender* renderer) override;
      bool OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS status) override;
      bool OnScenarioSimulationStatusChanged(surface_simulation::SIMULATION_STATUS status) override { return true; }
      bool OnScenarioTimeChanged(double time) override { return true; }
      bool OnScenarioModified() { m_chartUSN = INVALID_ID; return true; }

      iProperty* GetProperties() override;

   private:
      bool onScenarioCheckOpened();
      bool onScenarioMapProcessed();
      bool onScenarioMapObjProcessed();
      bool onScenarioPathFound();
      bool onScenarioOptPathFound();

      void initObjInfo();
      bool synchronize_map(render::iRender* renderer, const SV::surface_simulation::iSimulationState& state, const settings::environment_settings& env_stt);
      void addChartObject(render::iRender* renderer, const SV::surface_simulation::iLayerChartObject* obj);
      void addCoverage(render::iRender* renderer, const pathfinder::UnsignedMatrix& coverage, const settings::environment_settings& env_stt);
      void addExplication(render::iRender* renderer, const pathfinder::UnsignedMatrix& explication, std::function<unsigned long(size_t)> colorizer, const settings::environment_settings& env_stt);
      void onLayerEnabledChanged() override;
      bool isLayerEnabled() const override { return true; }
   private:

      std::unordered_map<SV::chart_object::OBJECT_TYPE, render::object_info> m_objInfo;

      iPropertyPtr _props;

      bool _showDepthChartObjects = true;
      iPropertyPtr _prop_showDepthChartObjects;
      void OnShowDepthChartObjectsChanged();

      float _minScale2ShowDepthObjects = 50.;
      iPropertyPtr _prop_minScale2ShowDepthObjects;
      size_t m_chartUSN = INVALID_ID;
   };
}