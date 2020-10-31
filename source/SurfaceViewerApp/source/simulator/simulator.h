#pragma once
#include "colreg/ModuleGuard.h"
#include "common/file_storage.h"
#include "common/central_class.h"
#include "navdisp/ComService.h"
#include "colreg\SimulatorInterface.h"

namespace SV::simulator
{
   surface_simulation::iSimulator* getSimulator();

   bool simulatorInit(navigation_dispatcher::iComService*);
   bool simulatorStep();
   bool simulatorStart();
   void simulatorStop(bool simulatorReset = false);
   central_pack* GetPack();

   ICommunicator* GetCommunicator();

   /*const surface_simulation::iShip* getShip(colreg::id_type id);
   colreg::ReleaseGuard<colreg::iChartObjects> getChartObject(colreg::OBJECT_TYPE chart_object_type, colreg::id_type object_id);*/
   CG::geo_point getCenter();
   /*void enableTrafficStatistic(bool enable);
   bool isTrafficStatisticEnabled();*/
}