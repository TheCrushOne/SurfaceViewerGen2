#pragma once

#include "scenario\ScenarioManager.h"
#include "RenderLayers.h"

namespace SV
{
   class ScenarioLayerBase
      : public ScenarioObserverBase
   {
   public:
      bool OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS status) override final;
      bool OnScenarioSimulationStatusChanged(surface_simulation::SIMULATION_STATUS status) override final;
      bool OnScenarioTimeChanged(double time) override final;
      bool OnScenarioModified() override final;
   protected:
      virtual bool onScenarioCheckOpened();
      virtual bool onScenarioMapProcessed();
      virtual bool onScenarioMapObjProcessed();
      virtual bool onScenarioPathFound();
      virtual bool onScenarioOptPathFound();
   };
}