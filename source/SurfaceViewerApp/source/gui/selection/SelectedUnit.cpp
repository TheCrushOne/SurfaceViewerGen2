#include "stdafx.h"
#include "SelectedUnit.h"

SelectedUnit::SelectedUnit(colreg::id_type id)
   : m_id{ id }
{
   auto selected = simulator::getShip(id);
   if (selected)
   {
      mountShipInfo(selected);
      mountTrackPointInfo(selected);
      mountSimSettings(selected);
      mountColregSettings(selected);
      mountSolution(selected);
      mountModel(selected);
   }
}