#pragma once
#include "SelectedObjectManager.h"
#include "colreg/SimulationStateInterface.h"

namespace SV
{
   class SelectedUnit
      : public iSelected
   {
   public:
      SelectedUnit(id_type id, surface_simulation::UNIT_TYPE type);
      void Render(render::iRender* renderer) override;
   protected:
      void mountUnitInfo(const surface_simulation::iLayerUnit* selected);
      void mountTrackPointInfo(const surface_simulation::iLayerUnit* selected);
      void mountSimSettings(const surface_simulation::iLayerUnit* selected);
      void mountColregSettings(const surface_simulation::iLayerUnit* selected);
      void mountSolution(const surface_simulation::iLayerUnit* selected);
      void mountModel(const surface_simulation::iLayerUnit* selected);

      const surface_simulation::iSimulationState& getState() const;

      virtual const surface_simulation::iLayerUnit* getSelectedUnit() const { return getState().GetUnitById(m_id); }
      virtual void renderDomain(render::iRender*) const = 0;
      virtual void renderTrack(render::iRender*) const = 0;
   protected:
      id_type m_id;
      layer_provider::ship_info m_info;

      std::unique_ptr<FolderPropertyHolder> m_ship_info_folder;
      iPropertyModifyPtr m_prop_id;
      iPropertyModifyPtr m_prop_ais;
      iPropertyModifyPtr m_prop_length;
      iPropertyModifyPtr m_prop_width;
      iPropertyModifyPtr m_prop_draft;
      iPropertyModifyPtr m_prop_safety_contour;
      iPropertyModifyPtr m_prop_deadweight;
      iPropertyModifyPtr m_prop_telegraph;
   };

   class SelectedDrone : public SelectedUnit
   {
   public:
      SelectedDrone(id_type id)
         : SelectedUnit(id, surface_simulation::UNIT_TYPE::UT_DRONE)
      {}
   protected:
      void renderDomain(render::iRender*) const override final;
      void renderTrack(render::iRender*) const override final;
   };

   class SelectedRover : public SelectedUnit
   {
   public:
      SelectedRover(id_type id)
         : SelectedUnit(id, surface_simulation::UNIT_TYPE::UT_ROVER)
      {}
   protected:
      void renderDomain(render::iRender*) const override final;
      void renderTrack(render::iRender*) const override final;
   };

   class SelectedShip : public SelectedUnit
   {
   public:
      SelectedShip(id_type id)
         : SelectedUnit(id, surface_simulation::UNIT_TYPE::UT_SHIP)
      {}
   protected:
      void renderDomain(render::iRender*) const override final;
      void renderTrack(render::iRender*) const override final;
   };
}