#include "stdafx.h"
#include "NavigationDispatcher.h"
#include "crossdllinterface\NavigationDispatcherInterface.h"
#include "ComService\ComServiceHolder.h"
//#include "xercesc\dom\DOMDocument.hpp"
//#include "DataStandartmanager/nd_command.hxx"


using namespace navigation_dispatcher;

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
      return false; \
   } \
   guard->Init(GetPack());

NavigationDispatcher::NavigationDispatcher(central_pack * pack)
   : Central(pack)
   //, m_manager(std::make_unique<data_standart::DataStandartManager>(pack))
{}

NavigationDispatcher::~NavigationDispatcher()
{}

int NavigationDispatcher::ProcessCommand(const char* configFilePath, const char* begCommandName, const char* baseFolder)
{
   //Logger logger(L"CommandServices.log");
   ComServiceHolder service(GetPack(), baseFolder);
   service.Get()->GetConfigDispatcher()->Dispatch(configFilePath);
   service.Get()->GetOrderProcessor()->ProcessOrders(begCommandName);
   //data_standart::command_meta meta;
   //m_manager->DeserializeConfig(configFilePath, meta);

   //service.Get()->GetDataStandartFactory()->SetScriptFolder(fpath(configFilePath).folder().get());

   //service.Get()->()->;

   //if (service.Get()->GetSettingsSerializerHolder())
      //return 1;
   //if (!m_manager->CheckMeta(meta))
      //return 1;
   //if (!m_manager->RunCommand(meta))
      //return 1;

   return 1;
}

extern "C" NDEXPRTIMPRT iNavigationDispatcher* CreateNavigationDispatcher(central_pack * pack)
{
   return new NavigationDispatcher(pack);
}