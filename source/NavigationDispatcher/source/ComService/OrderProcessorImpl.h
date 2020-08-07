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
   void AddOrder(navigation_dispatcher::iOrderPtr command) override final;
   void ClearOrders() override final;

   bool ProcessOrders(LPCWSTR begCommandName = NULL) override final;
protected:
   bool processOrder(LPCWSTR name, navigation_dispatcher::iOrderPtr& order);
private:
   navigation_dispatcher::iComService* m_service;
   std::map<std::wstring, navigation_dispatcher::iOrderPtr> m_commands;
   //std::map<std::wstring, std::shared_ptr<Logger>>  m_loggers;
};