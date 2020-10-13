#pragma once

#include "navdisp/OrderProcessor.h"
#include "navdisp/ComService.h"
#include <map>

class OrderProcessorImpl
   : public navigation_dispatcher::iOrderProcessor
   , private Central
{
public:
   OrderProcessorImpl(central_pack* pack, navigation_dispatcher::iComService* service)
      : Central(pack)
      , m_service(service)
   {}

   // iOrderProcessor
   void AddOrder(navigation_dispatcher::iOrder* command) override final;
   void ClearOrders() override final;

   bool ProcessOrders(LPCSTR begCommandName = NULL) override final;
protected:
   bool processOrder(LPCSTR name, navigation_dispatcher::iOrder* order);
private:
   navigation_dispatcher::iComService* m_service;
   std::map<std::string, navigation_dispatcher::iOrder*> m_commands;
   //std::map<std::wstring, std::shared_ptr<Logger>>  m_loggers;
};