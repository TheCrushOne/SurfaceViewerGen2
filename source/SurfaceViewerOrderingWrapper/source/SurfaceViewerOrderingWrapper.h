#pragma once

#include "colreg/ModuleGuard.h"
#include "crossdllinterface/SurfaceViewerOrderingWrapperInterface.h"
#include "crossdllinterface\NavigationDispatcherInterface.h"

namespace SV::surface_ordering
{
   class OrderingWrapper : public iOrderingWrapper, public Central
   {
   public:
      OrderingWrapper(central_pack* pack, const wchar_t* databasePath);

      void prepareCommandFromTemplate(std::wstring sourcePath, std::wstring dstPath, std::unordered_map<std::string, std::wstring> dict);

      bool ProcessOrder(const wchar_t* orderFile, const wchar_t* begCommand, std::unordered_map<std::string, std::wstring>& dict) override final;
      void Release() override { delete this; }
   private:
      system::ModuleGuard<navigation_dispatcher::iNavigationDispatcher, central_pack*> m_navigationDispatcher;
      std::string m_hashDatabasePath;

      std::wstring m_cacheFolder;
      std::wstring m_orderCacheFolder;
      std::wstring m_orderHeapFolder;
   };
}