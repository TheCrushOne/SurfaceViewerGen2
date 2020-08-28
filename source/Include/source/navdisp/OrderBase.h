#pragma once

#include "OrderInterface.h"
#include "OrderFactory.h"
#include "ComService.h"
#include "common/data_hash.h"

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
      bool DeserializeAttrs(const xml_properties::PropertyItem& params) override final { return m_commandData.Deserialize(params); }
      bool Process() override { return processCommand(); }

      bool recordOrderHashResult()
      {
         // NOTE: не записываем в этом случае
         if (m_commandData.rewrite_dst.AsBoolean())
            return true;
         auto hash = getHashData();
         m_pService->GetDatabaseController()->SaveDataStandartHashJunction(hash);
         return true;
      }
      bool needToProcess()
      {
         if (m_commandData.rewrite_dst.AsBoolean())
            return true;
         auto hash = getHashData();
         ATLASSERT(hash.source != data_hash::INVALID_HASH);
         if (hash.destination == data_hash::INVALID_HASH)
            return true;   // NOTE: случай, когда дест пустой
         bool result = m_pService->GetDatabaseController()->CheckDataStandartHashJunction(hash);
         if (result)
            GetCommunicator()->Message(ICommunicator::MessageType::MT_INFO, "Command skipped by hash junction control");
         return !result;
      }
   private:
      data_hash::hash_junction getHashData()
      {
         auto* src = m_pService->GetDataStandartFactory()->GetDataStandart(m_commandData.source.AsString());
         auto* dst = m_pService->GetDataStandartFactory()->GetDataStandart(m_commandData.destination.AsString());
         return data_hash::hash_junction{ src->GetDataHash(), dst->GetDataHash() };
      }
      virtual bool processCommand() = 0;
   protected:
      TOrderData m_commandData;
      iComService* m_pService;
      // NOTE: ThreadJobsManager -> check unidata
   };
}

#undef VALID_CHECK_DLL_LOAD