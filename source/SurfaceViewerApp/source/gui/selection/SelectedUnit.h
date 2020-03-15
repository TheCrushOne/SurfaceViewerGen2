#pragma once
#include "SelectedObjectManager.h"

class SelectedUnit : public iSelected
{
public:
   SelectedUnit(colreg::id_type id);
   void Render(render::iRender* renderer)override;

private:
   colreg::id_type m_id;
};

class SelectedDrone : public SelectedUnit
{

};

class SelectedRover : public SelectedUnit
{

};

class SelectedDrone : public SelectedUnit
{

};