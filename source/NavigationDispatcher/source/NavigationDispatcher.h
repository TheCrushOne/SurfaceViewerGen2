#pragma once

#include "crossdllinterface/NavigationDispatcherInterface.h"
#include "DataStandartManager\DataStandartManager.h"

namespace navigation_dispatcher
{
   class NavigationDispatcher : public iNavigationDispatcher, public Central
   {
   public:
      NavigationDispatcher(central_pack* pack);
      ~NavigationDispatcher();
   public:
      void Release() override final { delete this; }
      int ProcessCommand(const wchar_t* configFilePath, const wchar_t* begCommandName) override final;
   private:
      //std::unique_ptr<data_standart::DataStandartManager> m_manager;
   };
}