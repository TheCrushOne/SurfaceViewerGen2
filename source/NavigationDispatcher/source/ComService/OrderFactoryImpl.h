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
   navigation_dispatcher::iOrder* CreateOrder(navigation_dispatcher::OrderType type, LPCSTR id) override final;
   void DeleteOrder(LPCSTR id) override final;
   navigation_dispatcher::iOrder* GetOrder(LPCSTR id) const override final;
   void Clear() override final;
private:
   navigation_dispatcher::iComService* m_service;
   mutable std::unordered_map<std::string, OrderModuleGuard> m_orderMap;
};