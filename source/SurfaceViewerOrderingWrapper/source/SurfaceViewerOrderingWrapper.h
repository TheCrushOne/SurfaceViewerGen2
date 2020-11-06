#pragma once

#include "colreg/ModuleGuard.h"
#include "crossdllinterface/SurfaceViewerOrderingWrapperInterface.h"
#include "crossdllinterface\NavigationDispatcherInterface.h"

namespace SV::surface_ordering
{
   class OrderingWrapper
      : public iOrderingWrapper
      , public Central
   {
   public:
      OrderingWrapper(central_pack* pack, const char* databasePath);

      void prepareCommandFromTemplate(std::string sourcePath, std::string dstPath, std::unordered_map<std::string, std::string> dict);

      bool ProcessOrder(const char* orderFile, const char* begCommand, std::unordered_map<std::string, std::string>& dict) override final;
      void Release() override { delete this; }
   private:
      system::ModuleGuard<navigation_dispatcher::iNavigationDispatcher, central_pack*> m_navigationDispatcher;
      std::string m_hashDatabasePath;

      std::string m_cacheFolder;
      std::string m_orderCacheFolder;
      std::string m_orderHeapFolder;
   };
}