#pragma once

#include "common/Communicator.h"
#include "datastandart/DataStandartFactory.h"
#include "OrderFactory.h"
#include "settings/SettingsSerializerHolder.h"
#include "ConfigDispatcher.h"
#include "OrderProcessor.h"
#include "crossdllinterface\SVGMDatabaseInterface.h"

namespace navigation_dispatcher
{
   struct iComService
   {
      // NOTE: она дублируется с Communicator!!!
      //virtual ICommunicator* GetCommunicator() = 0;

      virtual iConfigDispatcher* GetConfigDispatcher() = 0;
      virtual settings::iSettingsSerializerHolder* GetSettingsSerializerHolder() = 0;
      virtual data_standart::iDataStandartFactory* GetDataStandartFactory() = 0;
      virtual iOrderFactory* GetOrderFactory() = 0;
      virtual iOrderProcessor* GetOrderProcessor() = 0;
      virtual database::iSVGMDatabaseController* GetDatabaseController() = 0;
   };
}