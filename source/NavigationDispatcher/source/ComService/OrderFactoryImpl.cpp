#include "stdafx.h"
#include "OrderFactoryImpl.h"
#include "navdisp/OrderCreation.h"

using namespace navigation_dispatcher;

OrderFactoryImpl::OrderFactoryImpl(central_pack* pack, iComService* service)
   : Central(pack)
   , m_service(service)
{
}

#define ORDER_CASE(type, method) case type: return std::shared_ptr<iOrder>(method(GetPack(), m_service));

#define VALID_CHECK_DLL_LOAD(type, dllName, funcName) case type: \
   m_orderMap[type] = OrderModuleGuard(); \
   m_orderMap[type].Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName, GetPack(), m_service); \
   if (!m_orderMap[type].IsValid()) \
   { \
      GetPack()->comm->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
   } \
   break;

iOrderPtr OrderFactoryImpl::CreateOrder(navigation_dispatcher::OrderType type)
{
   switch (type)
   {
      VALID_CHECK_DLL_LOAD(navigation_dispatcher::OrderType::OT_PNGHMCONVERT, "HeightMapToSVGMConverter", "CreatePngHeightmapConverter");
      VALID_CHECK_DLL_LOAD(navigation_dispatcher::OrderType::OT_GENOBJLIST, "ChartObjectGenerator", "CreateObjectListGenerator");
      VALID_CHECK_DLL_LOAD(navigation_dispatcher::OrderType::OT_PATHFIND, "Engine", "CreatePathfinder");
      VALID_CHECK_DLL_LOAD(navigation_dispatcher::OrderType::OT_OPTPATH, "Engine", "CreateOptimizedPathfinder");
      VALID_CHECK_DLL_LOAD(navigation_dispatcher::OrderType::OT_PACKHOUND, "PackHound", "CreatePackHound");
   }

   _ASSERT(!"Unknown command type!");
   return std::shared_ptr<iOrder>(NULL);
}

#undef ORDER_CASE
#undef VALID_CHECK_DLL_LOAD