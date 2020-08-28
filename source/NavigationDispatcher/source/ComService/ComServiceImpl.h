#pragma once

#include "common/communicator.h"
#include "navdisp/ComService.h"
#include "DataStandartFactoryImpl.h"
#include "OrderFactoryImpl.h"
#include "OrderProcessorImpl.h"
#include "ConfigDispatcherImpl.h"
#include "SettingsSerializerHolderImpl.h"
#include "crossdllinterface\SVGMDatabaseInterface.h"
/*#include "FilterFactoryImpl.h"
#include "DataSerializerImpl.h"
#include "DataSourceFactoryImpl.h"
#include "CommandFactoryImpl.h"
#include "CommandProcessorImpl.h"
#include "MetadataAccessorImpl.h"
#include "CommonBuildParamsHolderImpl.h"*/


#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard, ...) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName, __VA_ARGS__); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
      return; \
   }

namespace navigation_dispatcher
{
   class CommandServicesImpl : public iComService, public Central
   {
   public:
      CommandServicesImpl(central_pack* pack, const char* baseFolder)
         : Central(pack)
         , m_dataStandartFactory(std::make_shared<DataStandartFactoryImpl>(pack, this))
         , m_orderFactory(std::make_shared<OrderFactoryImpl>(pack, this))
         , m_configDispatcher(std::make_shared<ConfigDispatcherImpl>(pack, this))
         , m_settingsSerializer(std::make_shared<SettingsSerializerHolderImpl>(pack, this))
         , m_orderProcessor(std::make_shared<OrderProcessorImpl>(pack, this))
         /*: m_filterFactory(this)
         , m_dataSerializer(comm)
         , m_dataSourceFactory(this)
         , m_commandFactoryImpl(this)
         , m_commandProcessor(this)
         , m_metadataAccessor(this)*/
      {
         VALID_CHECK_DLL_LOAD("SQLiteController", "CreateSQLiteDatabaseController", m_databaseController, pack);
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
      //iCommandService
      /*iFilterFactory* GetFilterFactory() final
      {
         return &m_filterFactory;
      }
      iDataSerializer* GetDataSerializer() final
      {
         return &m_dataSerializer;
      }
      iDataSourceFactory* GetDataSourceFactory() final
      {
         return &m_dataSourceFactory;
      }
      iCommandFactory* GetCommandFactory() final
      {
         return &m_commandFactoryImpl;
      }
      iCommandProcessor* GetCommandProcessor() final
      {
         return &m_commandProcessor;
      }
      iCommonBuildParamsHolder* GetCommonBuildParamsHolder() final
      {
         return &m_commonBuildParamsHolder;
      }
      iMetadataAccessor* GetMetadataAccessor() final
      {
         return &m_metadataAccessor;
      }*/

   private:
      std::shared_ptr<DataStandartFactoryImpl>                 m_dataStandartFactory;
      std::shared_ptr<OrderFactoryImpl>                        m_orderFactory;
      std::shared_ptr<ConfigDispatcherImpl>                    m_configDispatcher;
      std::shared_ptr<SettingsSerializerHolderImpl>            m_settingsSerializer;
      std::shared_ptr<OrderProcessorImpl>                      m_orderProcessor;
      colreg::ModuleGuard<database::iSVGMDatabaseController, central_pack*>   m_databaseController;
      /*FilterFactoryImpl           m_filterFactory;
      DataSerializerImpl          m_dataSerializer;
      DataSourceFactoryImpl       m_dataSourceFactory;
      CommandFactoryImpl          m_commandFactoryImpl;
      CommandProcessorImpl        m_commandProcessor;
      MetadataAccessorImpl        m_metadataAccessor;
      CommonBuildParamsHolderImpl m_commonBuildParamsHolder;*/
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