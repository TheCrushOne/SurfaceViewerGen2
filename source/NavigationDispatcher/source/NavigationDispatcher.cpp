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

int NavigationDispatcher::ProcessCommand(const char* configFilePath, const char* begCommandName, const char* baseFolder, iComService* service)
{
   //Logger logger(L"CommandServices.log");
   if (service)
   {
      service->GetConfigDispatcher()->Dispatch(configFilePath);
      service->GetOrderProcessor()->ProcessOrders(begCommandName);
   }
   else
   {
      ComServiceHolder mservice(GetPack(), baseFolder);
      mservice.Get()->GetConfigDispatcher()->Dispatch(configFilePath);
      mservice.Get()->GetOrderProcessor()->ProcessOrders(begCommandName);
   }

   return 1;
}

extern "C" NDEXPRTIMPRT iNavigationDispatcher* CreateNavigationDispatcher(central_pack * pack)
{
   return new NavigationDispatcher(pack);
}