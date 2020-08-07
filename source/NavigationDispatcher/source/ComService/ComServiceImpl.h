#pragma once

#include "common/communicator.h"
#include "navdisp/ComService.h"
#include "DataStandartFactoryImpl.h"
#include "OrderFactoryImpl.h"
#include "OrderProcessorImpl.h"
#include "ConfigDispatcherImpl.h"
#include "SettingsSerializerHolderImpl.h"
/*#include "FilterFactoryImpl.h"
#include "DataSerializerImpl.h"
#include "DataSourceFactoryImpl.h"
#include "CommandFactoryImpl.h"
#include "CommandProcessorImpl.h"
#include "MetadataAccessorImpl.h"
#include "CommonBuildParamsHolderImpl.h"*/

namespace navigation_dispatcher
{
   class CommandServicesImpl : public iComService, public Central
   {
   public:
      CommandServicesImpl(central_pack* pack)
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
      {}
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
      std::shared_ptr<DataStandartFactoryImpl>           m_dataStandartFactory;
      std::shared_ptr<OrderFactoryImpl>                  m_orderFactory;
      std::shared_ptr<ConfigDispatcherImpl>              m_configDispatcher;
      std::shared_ptr<SettingsSerializerHolderImpl>      m_settingsSerializer;
      std::shared_ptr<OrderProcessorImpl>                m_orderProcessor;
      /*FilterFactoryImpl           m_filterFactory;
      DataSerializerImpl          m_dataSerializer;
      DataSourceFactoryImpl       m_dataSourceFactory;
      CommandFactoryImpl          m_commandFactoryImpl;
      CommandProcessorImpl        m_commandProcessor;
      MetadataAccessorImpl        m_metadataAccessor;
      CommonBuildParamsHolderImpl m_commonBuildParamsHolder;*/
   };

   // extern functions implementation

   iComService* CreateCommandServices(central_pack* pack)
   {
      return new CommandServicesImpl(pack);
   }

   void DestroyCommandServices(iComService* commandServices)
   {
      CommandServicesImpl* commandServicesImpl = dynamic_cast<CommandServicesImpl*>(commandServices);
      _ASSERT(commandServicesImpl != NULL);
      delete commandServicesImpl;
   }
}