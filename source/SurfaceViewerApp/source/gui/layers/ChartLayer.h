#pragma once

#include "RenderLayers.h"
#include "layer_provider\layer_chart_object.h"
#include "common\chart_object_unit.h"
#include "scenario\ScenarioManager.h"
#include "ScenarioLayerBase.h"
#include "gui\helpers\LayerFiltersManager.h"

namespace SV
{
   class ChartLayer
      : public LayersContainer
      , public ScenarioLayerBase
   {
      struct layer_filter_tag
      {
         static constexpr char general[] = "General";

         static constexpr char explications[] = "Explications";
         static constexpr char land[] = "Land";
         static constexpr char air[] = "Air";

         static constexpr char coverages[] = "Coverages";

         static constexpr char step_templ[] = "Step_";

         static constexpr char chart_objects[] = "Chart Objects";

         static constexpr char isolines[] = "Isolines";
         static constexpr char border_areas[] = "Border Areas";
         static constexpr char nogo_zones[] = "NoGo Zones";
         static constexpr char nofly_zones[] = "NoFly Zones";

         static constexpr char isoline_templ[] = "Level ";
         static constexpr char border_area_templ[] = "Border Area ";
         static constexpr char nogo_zone_templ[] = "NoGo Zone ";
         static constexpr char nofly_zone_templ[] = "NoFly Zone ";
      };
   public:
      ChartLayer() { initObjInfo(); }
      void Render(render::iRender* renderer) override;
      iProperty* GetProperties() override;
   protected:
      bool onScenarioCheckOpened() override final;
      bool onScenarioMapObjProcessed() override final;
      bool onScenarioPathFound() override final;
      bool onScenarioOptPathFound() override final;
   private:
      bool onAnyPathFound();
      void initObjInfo();
      void updateCoverageFilterList();
      bool synchronize_map(render::iRender* renderer, const SV::surface_simulation::iSimulationState& state, const settings::environment_settings& env_stt);
      void addChartObject(render::iRender* renderer, const SV::surface_simulation::iLayerChartObject* obj);
      void addCoverage(render::iRender* renderer, const pathfinder::UnsignedMatrix& coverage, const settings::environment_settings& env_stt);
      void addExplication(render::iRender* renderer, const pathfinder::UnsignedMatrix& explication, std::function<unsigned long(size_t)> colorizer, const settings::environment_settings& env_stt);
      void onLayerEnabledChanged() override;
      bool isLayerEnabled() const override { return true; }

      layer_filter_path formatExplicationPath();
      layer_filter_path formatAirExplicationPath();
      layer_filter_path formatLandExplicationPath();

      layer_filter_path formatCoveragesPath();
      layer_filter_path formatCoveragesStepIdxPath(size_t idx);

      layer_filter_path formatChartObjectsPath();
      layer_filter_path formatChartObjectsIsolinesPath();
      layer_filter_path formatChartObjectsIsolinesPathId(chart_object_id id, double height);
      layer_filter_path formatChartObjectsBorderAreasPath();
      layer_filter_path formatChartObjectsBorderAreasPathId(chart_object_id id);
      layer_filter_path formatChartObjectsNogoZonesPath();
      layer_filter_path formatChartObjectsNogoZonesPathId(chart_object_id id);
      layer_filter_path formatChartObjectsNoFlyZonesPath();
      layer_filter_path formatChartObjectsNoFlyZonesPathId(chart_object_id id);

      double getIsolineHeight(const properties::simple_prop_vct* prop_vct);
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