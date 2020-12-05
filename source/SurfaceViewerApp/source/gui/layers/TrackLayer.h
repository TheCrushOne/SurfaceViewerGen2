#pragma once
#include "RenderLayers.h"
#include "colreg/PropertyInterface.h"
#include "properties\PropertyModify.h"
#include "scenario\ScenarioManager.h"
#include "ScenarioLayerBase.h"
#include "gui\helpers\LayerFiltersManager.h"

namespace SV
{
   class TrackLayer
      : public LayersContainer
      , public ScenarioLayerBase
   {
      struct layer_filter_tag
      {
         static constexpr char drones[] = "Drones";
         static constexpr char rovers[] = "Rovers";

         static constexpr char drone_templ[] = "Drone ";
         static constexpr char rover_templ[] = "Rover ";

         static constexpr char control_points[] = "Control Points";
         static constexpr char source_route[] = "Source Route";

         static constexpr char segments[] = "Segments";
         static constexpr char points[] = "Points";

         static constexpr char segment_templ[] = "Segment ";
         static constexpr char point_templ[] = "Point ";
      };
   public:
      TrackLayer()
         : m_step{ 1. }
      {}

      void SetStep(double step) { m_step = step; }

      void Render(render::iRender* renderer) override
      {
         renderTracks(renderer);
      }
   protected:
      bool onScenarioPathFound() override final;
      bool onScenarioOptPathFound() override final;
   private:
      void renderTracks(render::iRender* renderer);
      bool onAnyPathFound();

      layer_filter_path formatIdDronePath(SV::id_type id);

      layer_filter_path formatIdDroneSrcRoutePath(SV::id_type id);
      layer_filter_path formatIdDroneSrcRouteSegmentsPath(SV::id_type id);
      layer_filter_path formatIdDroneSrcRouteSegmentsIdxPath(SV::id_type id, size_t idx);
      layer_filter_path formatIdDroneSrcRoutePointsPath(SV::id_type id);
      layer_filter_path formatIdDroneSrcRoutePointsIdxPath(SV::id_type id, size_t idx, CG::route_point point);

      layer_filter_path formatIdDroneSrcControlPointsPath(SV::id_type id);
      layer_filter_path formatIdDroneSrcControlPointsSegmentsPath(SV::id_type id);
      layer_filter_path formatIdDroneSrcControlPointsSegmentsIdxPath(SV::id_type id, size_t idx);
      layer_filter_path formatIdDroneSrcControlPointsPointsPath(SV::id_type id);
      layer_filter_path formatIdDroneSrcControlPointsPointsIdxPath(SV::id_type id, size_t idx, CG::route_point point);

      layer_filter_path formatIdRoverPath(SV::id_type id);

      layer_filter_path formatIdRoverSrcRoutePath(SV::id_type id);
      layer_filter_path formatIdRoverSrcRouteSegmentsPath(SV::id_type id);
      layer_filter_path formatIdRoverSrcRouteSegmentsIdxPath(SV::id_type id, size_t idx);
      layer_filter_path formatIdRoverSrcRoutePointsPath(SV::id_type id);
      layer_filter_path formatIdRoverSrcRoutePointsIdxPath(SV::id_type id, size_t idx, CG::route_point point);

      layer_filter_path formatIdRoverSrcControlPointsPath(SV::id_type id);
      layer_filter_path formatIdRoverSrcControlPointsSegmentsPath(SV::id_type id);
      layer_filter_path formatIdRoverSrcControlPointsSegmentsIdxPath(SV::id_type id, size_t idx);
      layer_filter_path formatIdRoverSrcControlPointsPointsPath(SV::id_type id);
      layer_filter_path formatIdRoverSrcControlPointsPointsIdxPath(SV::id_type id, size_t idx, CG::route_point point);
   private:
      double m_step;

      bool m_renderSourceRoute = true;
      bool m_renderControlPoints = true;

      iPropertyPtr m_prop_renderSourceRoute;
      iPropertyPtr m_prop_renderControlPoints;
   };
}