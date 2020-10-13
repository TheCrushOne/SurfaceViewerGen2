#pragma once

#include "datastandart\SVGenMapDataStandartInterface.h"
#include "datastandart\OptimizedPathStorageDataStandartInterface.h"
#include "engine.h"
#include "navdisp\OrderBase.h"
#include "navdisp\OrderStruct.h"

namespace engine
{
   class OptPathfinderExternal
      : public navigation_dispatcher::OrderBase<navigation_dispatcher::OrderType::OT_OPTPATH, navigation_dispatcher::opt_pathfind_order>
   {
   public:
      OptPathfinderExternal(central_pack* pack, navigation_dispatcher::iComService* pService);
      void Release() override final { delete this; }
   private:
      virtual bool processCommand() override final;
   private:
      bool readFromSource(data_standart::iSurfaceVieverGenMapDataStandart*);
      bool writeToDestination(data_standart::iOptimizedPathStorageDataStandart*);
      bool processData();
   private:
      pathfinder::GeoMatrix m_data;
      ColregSimulation::scenario_data m_scenarioData;
      std::shared_ptr<engine::Engine> m_engine;
      std::shared_ptr<settings::application_settings> m_settings;
   };
}