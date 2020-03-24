#pragma once
#include "colreg/ModuleGuard.h"
#include "colreg/ColregSimulation.h"
#include "common/file_storage.h"
#include "common/central_class.h"

namespace simulator
{
   ColregSimulation::iSimulator* getSimulator();

   bool simulatorStep();
   bool simulatorStart();
   void simulatorStop(bool simulatorReset = false);
   std::shared_ptr<central_pack> GetPack();

   ICommunicator* GetCommunicator();

   /*const ColregSimulation::iShip* getShip(colreg::id_type id);
   colreg::ReleaseGuard<colreg::iChartObjects> getChartObject(colreg::OBJECT_TYPE chart_object_type, colreg::id_type object_id);*/
   colreg::geo_point getCenter();
   /*void enableTrafficStatistic(bool enable);
   bool isTrafficStatisticEnabled();*/
}