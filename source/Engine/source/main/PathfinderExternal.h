#pragma once

#include "datastandart\SVGenMapDataStandartInterface.h"
#include "datastandart\PathStorageDataStandartInterface.h"
#include "engine.h"
#include "navdisp\OrderBase.h"
#include "navdisp\OrderStruct.h"

namespace engine
{
   class PathfinderExternal
      : public navigation_dispatcher::OrderBase<navigation_dispatcher::OrderType::OT_PATHFIND, navigation_dispatcher::pathfind_order>
   {
   public:
      PathfinderExternal(central_pack* pack, navigation_dispatcher::iComService* pService);
      void Release() override final { delete this; }
   private:
      virtual bool processCommand() override final;
   private:
      bool readFromSource(data_standart::iSurfaceVieverGenMapDataStandart*);
      bool writeToDestination(data_standart::iPathStorageDataStandart*);
      bool processData();
   private:
      pathfinder::GeoMatrix m_data;
      ColregSimulation::scenario_data m_scenarioData;
      std::shared_ptr<engine::Engine> m_engine;
      std::shared_ptr<settings::application_settings> m_settings;
   };
}