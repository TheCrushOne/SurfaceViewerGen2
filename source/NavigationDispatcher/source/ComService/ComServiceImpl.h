#pragma once

#include "common/communicator.h"
#include "navdisp/ComService.h"
#include "DataStandartFactoryImpl.h"
#include "OrderFactoryImpl.h"
#include "OrderProcessorImpl.h"
#include "ConfigDispatcherImpl.h"
#include "SettingsSerializerHolderImpl.h"

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard, ...) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName, __VA_ARGS__); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(SV::ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
      return; \
   }

namespace SV::navigation_dispatcher
{
   class CommandServicesImpl
      : public iComService
      , public Central
   {
   public:
      CommandServicesImpl(central_pack* pack, const char* baseFolder)
         : Central(pack)
         , m_dataStandartFactory(std::make_shared<DataStandartFactoryImpl>(pack, this))
         , m_orderFactory(std::make_shared<OrderFactoryImpl>(pack, this))
         , m_configDispatcher(std::make_shared<ConfigDispatcherImpl>(pack, this))
         , m_settingsSerializer(std::make_shared<SettingsSerializerHolderImpl>(pack, this))
         , m_orderProcessor(std::make_shared<OrderProcessorImpl>(pack, this))
      {
         VALID_CHECK_DLL_LOAD("SettingsHandler", "CreateUnitDataSerializer", m_unitDataSerializer);
         VALID_CHECK_DLL_LOAD("ChecksumService", "CreateChecksumService", m_checksumService, pack);
         VALID_CHECK_DLL_LOAD("SQLiteController", "CreateSQLiteDatabaseController", m_databaseController, pack);
         VALID_CHECK_DLL_LOAD("PythonMiscellaneousWrapper", "CreatePythonWrapper", m_pythonWrapper, pack);
         auto pathToDb = std::string(baseFolder) + "\\svgm.db";
         m_databaseController->Connect(pathToDb.c_str());
      }
      settings::iSettingsSerializerHolder* GetSettingsSerializerHolder() override final
      {
         return m_settingsSerializer.get();
      }
      data_standart::iDataStandartFactory* GetDataStandartFactory() override final
      {
         return m_dataStandartFactory.get();
      }
      iOrderFactory* GetOrderFactory() override final
      {
         return m_orderFactory.get();
      }
      iConfigDispatcher* GetConfigDispatcher() override final
      {
         return m_configDispatcher.get();
      }
      iOrderProcessor* GetOrderProcessor() override final
      {
         return m_orderProcessor.get();
      }
      database::iSVGMDatabaseController* GetDatabaseController() override final
      {
         return m_databaseController;
      }
      checksum::iChecksumService* GetChecksumService() override final
      {
         return m_checksumService;

      }
      serializer::iUnitDataSerializer* GetUnitDataSerializer() override final
      {
         return m_unitDataSerializer;
      }
      python_wrapper::iPythonWrapper* GetPythonWrapper() override final
      {
         return m_pythonWrapper;
      }
      void Release() override final { delete this; }
   private:
      SharedDataStandartFactoryImpl                                              m_dataStandartFactory;
      SharedOrderFactoryImpl                                                     m_orderFactory;
      SharedConfigDispatcherImpl                                                 m_configDispatcher;
      SharedSettingsSerializerHolderImpl                                         m_settingsSerializer;
      SharedOrderProcessorImpl                                                   m_orderProcessor;
      system::ModuleGuard<database::iSVGMDatabaseController, central_pack*>      m_databaseController;
      system::ModuleGuard<checksum::iChecksumService, central_pack*>             m_checksumService;
      system::ModuleGuard<serializer::iUnitDataSerializer>                       m_unitDataSerializer;
      system::ModuleGuard<python_wrapper::iPythonWrapper, central_pack*>         m_pythonWrapper;
   };

   // extern functions implementation

   iComService* CreateCommandServices(central_pack* pack, const char* baseFolder)
   {
      return new CommandServicesImpl(pack, baseFolder);
   }

   void DestroyCommandServices(iComService* commandServices)
   {
      CommandServicesImpl* commandServicesImpl = dynamic_cast<CommandServicesImpl*>(commandServices);
      _ASSERT(commandServicesImpl != NULL);
      delete commandServicesImpl;
   }
}

extern "C" NDEXPRTIMPRT SV::navigation_dispatcher::iComService * CreateExternalComService(SV::central_pack* pack, const char* baseFolder)
{
   return new SV::navigation_dispatcher::CommandServicesImpl(pack, baseFolder);
}