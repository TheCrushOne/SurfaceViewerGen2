#pragma once

#include "crossdllinterface/NavigationDispatcherInterface.h"
#include "DataStandartManager\DataStandartManager.h"

namespace SV::navigation_dispatcher
{
   class NavigationDispatcher
      : public iNavigationDispatcher
      , public Central
   {
   public:
      NavigationDispatcher(central_pack* pack);
      ~NavigationDispatcher();
   public:
      void Release() override final { delete this; }
      int ProcessCommand(const char*, const char*, const char*, iComService* service = nullptr) override final;
   private:
      //std::unique_ptr<data_standart::DataStandartManager> m_manager;
   };
}