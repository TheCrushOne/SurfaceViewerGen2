#pragma once
#include "RenderLayers.h"
#include "properties\PropertyModify.h"
#include "scenario\ScenarioManager.h"
#include "ScenarioLayerBase.h"
#include "gui\helpers\LayerFiltersManager.h"

namespace SV
{
   class UnitLayer
      : public LayersContainer
      , public ScenarioLayerBase
   {
      struct layer_filter_tag
      {
         static constexpr char drones[] = "Drones";
         static constexpr char rovers[] = "Rovers";

         static constexpr char drone_templ[] = "Drone ";
         static constexpr char rover_templ[] = "Rover ";
      };
   public:
      void Render(render::iRender* renderer) override;
   protected:
      bool onScenarioPathFound() override final;
      bool onScenarioOptPathFound() override final;
   protected:
      void renderRovers(render::iRender* renderer);
      void renderDrones(render::iRender* renderer);
      void renderShips(render::iRender* renderer);
   private:
      bool onAnyPathFound();
      layer_filter_path formatDroneLayerFilterPath(id_type id);
      layer_filter_path formatRoverLayerFilterPath(id_type id);
      //void renderSuggestions(render::iRender* renderer, const colreg::statefull::suggestions_ref* suggestions);

      bool m_renderVector = true;
      iPropertyPtr m_prop_renderVector;

      double m_arrowDist = 0.5;
   };
}