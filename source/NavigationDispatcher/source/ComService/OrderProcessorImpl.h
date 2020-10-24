#pragma once

#include "navdisp/OrderProcessor.h"
#include "navdisp/ComService.h"
#include <map>

namespace SV::navigation_dispatcher
{
   class OrderProcessorImpl
      : public iOrderProcessor
      , private Central
   {
   public:
      OrderProcessorImpl(central_pack* pack, iComService* service)
         : Central(pack)
         , m_service(service)
      {}

      // iOrderProcessor
      void AddOrder(iOrder* command) override final;
      void ClearOrders() override final;

      bool ProcessOrders(LPCSTR begCommandName = NULL) override final;
   protected:
      bool processOrder(LPCSTR name, iOrder* order);
   private:
      iComService* m_service;
      std::map<std::string, iOrder*> m_commands;
      //std::map<std::wstring, std::shared_ptr<Logger>>  m_loggers;
   };

   typedef std::shared_ptr<OrderProcessorImpl> SharedOrderProcessorImpl;
}