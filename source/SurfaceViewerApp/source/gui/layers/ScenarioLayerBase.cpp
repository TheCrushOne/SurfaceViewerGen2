#include "stdafx.h"
#include "ScenarioLayerBase.h"

using namespace SV;

bool ScenarioLayerBase::OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS status)
{
   bool res = true;
   switch (status)
   {
   case surface_simulation::SCENARIO_STATUS::SS_MAP_CHECKOPENED:
      res &= onScenarioCheckOpened();
      break;
   case surface_simulation::SCENARIO_STATUS::SS_MAP_PROCESSED:
      res &= onScenarioMapProcessed();
      break;
   case surface_simulation::SCENARIO_STATUS::SS_MAPOBJ_PROCESSED:
      res &= onScenarioMapObjProcessed();
      break;
   case surface_simulation::SCENARIO_STATUS::SS_PATHS_COUNTED:
      res &= onScenarioPathFound();
      break;
   case surface_simulation::SCENARIO_STATUS::SS_OPT_PATHS_COUNTED:
      res &= onScenarioOptPathFound();
      break;
   case surface_simulation::SCENARIO_STATUS::SS_NOT_LOADED:
   default:
      break;
   }
   return res;
}

bool ScenarioLayerBase::OnScenarioSimulationStatusChanged(surface_simulation::SIMULATION_STATUS status)
{
   return true;
}

bool ScenarioLayerBase::OnScenarioTimeChanged(double time)
{
   return true;
}

bool ScenarioLayerBase::OnScenarioModified()
{
   return true;
}

bool ScenarioLayerBase::onScenarioCheckOpened()
{
   return true;
}

bool ScenarioLayerBase::onScenarioMapProcessed()
{
   return true;
}

bool ScenarioLayerBase::onScenarioMapObjProcessed()
{
   return true;
}

bool ScenarioLayerBase::onScenarioPathFound()
{
   return true;
}

bool ScenarioLayerBase::onScenarioOptPathFound()
{
   return true;
}