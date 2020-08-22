#pragma once

#include "OrderInterface.h"
#include "OrderFactory.h"
#include "ComService.h"
#include "crossdllinterface\SVGMDatabaseInterface.h"

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      m_lock = true; \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
      return;


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
      {
         VALID_CHECK_DLL_LOAD("SQLiteController", "CreateSQLiteDatabaseController", m_databaseController);
      }
   protected:
      OrderType GetType() const override { return static_cast<OrderType>(command_type); }
      bool IsMultithread() const override { return false; }
      void SetThreadNum(unsigned int threadNum) override {}
      bool DeserializeAttrs(const xml_properties::PropertyItem& params) override final { return m_commandData.Deserialize(params); }
      bool Process() override { return processCommand(); }
      bool RecordOrderResult()
      {
         auto hash = getHashData();
         m_databaseController->
      }
   private:
      virtual hash_junction getHashData() = 0;
      virtual bool processCommand() = 0;
   protected:
      colreg::ModuleGuard<database::iSVGMDatabaseController> m_databaseController;
      TOrderData m_commandData;
      iComService* m_pService;
      // NOTE: ThreadJobsManager -> check unidata
   };
}

#undef VALID_CHECK_DLL_LOAD