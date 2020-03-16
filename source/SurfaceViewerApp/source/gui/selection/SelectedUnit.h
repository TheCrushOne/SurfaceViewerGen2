#pragma once
#include "SelectedObjectManager.h"
#include "colreg/ColregSimulation.h"

class SelectedUnit : public iSelected
{
public:
   SelectedUnit(colreg::id_type id);
   void Render(render::iRender* renderer) override;
protected:
   void mountUnitInfo(const ColregSimulation::iUnit& selected);
   void mountTrackPointInfo(const ColregSimulation::iUnit& selected);
   void mountSimSettings(const ColregSimulation::iUnit& selected);
   void mountColregSettings(const ColregSimulation::iUnit& selected);
   void mountSolution(const ColregSimulation::iUnit& selected);
   void mountModel(const ColregSimulation::iUnit& selected);

   const ColregSimulation::iSimulationState& getState() const;

   virtual const ColregSimulation::iUnit& getSelectedUnit(colreg::id_type id) const = 0;
private:
   colreg::id_type m_id;
   colreg::ship_info m_info;

   std::unique_ptr<FolderProperty> m_ship_info_folder;
   iPropertyPtr m_prop_id;
   iPropertyPtr m_prop_ais;
   iPropertyPtr m_prop_length;
   iPropertyPtr m_prop_width;
   iPropertyPtr m_prop_draft;
   iPropertyPtr m_prop_safety_contour;
   iPropertyPtr m_prop_deadweight;
   iPropertyPtr m_prop_telegraph;
};

class SelectedDrone : public SelectedUnit
{
public:
   SelectedDrone(colreg::id_type id)
      : SelectedUnit(id)
   {}
protected:
   const ColregSimulation::iUnit& getSelectedUnit(colreg::id_type id) const override final { return getState().GetUnit(ColregSimulation::UNIT_TYPE::UT_DRONE, id); }
};

class SelectedRover : public SelectedUnit
{
public:
   SelectedRover(colreg::id_type id)
      : SelectedUnit(id)
   {}
protected:
   const ColregSimulation::iUnit& getSelectedUnit(colreg::id_type id) const override final { return getState().GetUnit(ColregSimulation::UNIT_TYPE::UT_ROVER, id); }
};

class SelectedShip : public SelectedUnit
{
public:
   SelectedShip(colreg::id_type id)
      : SelectedUnit(id)
   {}
protected:
   const ColregSimulation::iUnit& getSelectedUnit(colreg::id_type id) const override final { return getState().GetUnit(ColregSimulation::UNIT_TYPE::UT_SHIP, id); }
};