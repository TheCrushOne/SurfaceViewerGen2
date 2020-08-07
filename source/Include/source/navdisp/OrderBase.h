#pragma once

#include "OrderInterface.h"
#include "OrderFactory.h"
#include "ComService.h"

namespace navigation_dispatcher
{
   template <int TOrderType, typename TOrderData>
   class OrderBase : public iOrder, public Central
   {
      enum { command_type = TOrderType };
   public:
      OrderBase(central_pack* pack, iComService* service)
         : Central(pack)
         , m_pService(service)
      {}
   protected:
      OrderType GetType() const override { return static_cast<OrderType>(command_type); }
      bool IsMultithread() const override { return false; }
      void SetThreadNum(unsigned int threadNum) override {}
      bool Process() override { return processCommand(); }
   private:
      virtual bool processCommand() = 0;
   protected:
      TOrderData m_commandData;
      iComService* m_pService;
      // NOTE: ThreadJobsManager -> check unidata
   };
}