#pragma once

#include "datastandart\SVGenMapDataStandartInterface.h"
#include "datastandart\OptimizedPathStorageDataStandartInterface.h"
#include "pathfinding/PathfindingEngine.h"
#include "research\ResearchEngine.h"
#include "navdisp\OrderBase.h"
#include "navdisp\OrderStruct.h"

namespace SV::engine
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
      pathfinder::path_finder_indata m_indata;
      std::shared_ptr<engine::PathfindingEngine> m_pathfindingEngine;
      std::shared_ptr<engine::ResearchEngine> m_researchEngine;
      std::shared_ptr<settings::application_settings> m_settings;
   };
}