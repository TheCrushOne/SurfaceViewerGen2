#pragma once

#include "navdisp/OrderFactory.h"
#include "navdisp/ComService.h"
#include "colreg\ModuleGuard.h"
#include "navdisp\OrderInterface.h"

using OrderModuleGuard = colreg::ModuleGuard<navigation_dispatcher::iOrder, central_pack*, navigation_dispatcher::iComService*>;

class OrderFactoryImpl
   : public navigation_dispatcher::iOrderFactory
   , private Central
{
public:
   OrderFactoryImpl(central_pack* pack, navigation_dispatcher::iComService* services);

   // iCommandFactory
private:
   navigation_dispatcher::iOrderPtr CreateOrder(navigation_dispatcher::OrderType type) override final;

private:
   navigation_dispatcher::iComService* m_service;
   mutable std::unordered_map<navigation_dispatcher::OrderType, OrderModuleGuard> m_orderMap;
};