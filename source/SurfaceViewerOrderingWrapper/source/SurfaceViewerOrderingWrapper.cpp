#include "stdafx.h"
#include "SurfaceViewerOrderingWrapper.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <regex>

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard, ...) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName, __VA_ARGS__); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
   }

using namespace surface_ordering;

OrderingWrapper::OrderingWrapper(central_pack_ptr pack, const wchar_t* databasePath)
   : Central(pack)
   , m_hashDatabasePath(SVGUtils::wstringToString(databasePath))
{
   m_cacheFolder = L"../../../cache/";
   m_orderCacheFolder = m_cacheFolder + L"order_heap/";
   m_orderHeapFolder = L"../../../heap/";

   VALID_CHECK_DLL_LOAD("NavigationDispatcher", "CreateNavigationDispatcher", m_navigationDispatcher, pack);
}

void OrderingWrapper::prepareCommandFromTemplate(std::wstring sourcePath, std::wstring dstPath, std::unordered_map<std::string, std::wstring> dict)
{
   std::filesystem::copy_file(sourcePath, dstPath);
   std::ifstream file(dstPath, std::ios::in);
   std::ostringstream out;
   std::string fileData;
   out << file.rdbuf();
   fileData = out.str();
   auto replace = [](std::string& src, const std::string& token, const std::string& value)
   {
      src = std::regex_replace(src, std::regex(token), value);
   };
   for (auto& dictPair : dict)
   {
      std::string token = "$(" + dictPair.first + ")";
      replace(fileData, token, SVGUtils::wstringToString(dictPair.second));
   }
   file.close();

   std::ofstream ofile(dstPath, std::ios::out);
   ofile << fileData;
}

bool OrderingWrapper::ProcessOrder(const wchar_t* orderFileName, const wchar_t* begCommand, std::unordered_map<std::string, std::wstring>& dict)
{
   std::wstring srcOrderPath = m_orderHeapFolder + orderFileName;
   std::wstring dstOrderPath = m_orderCacheFolder + orderFileName;
   prepareCommandFromTemplate(srcOrderPath, dstOrderPath, dict);
   return m_navigationDispatcher->ProcessCommand(SVGUtils::wstringToString(dstOrderPath).c_str(), begCommand ? SVGUtils::wstringToString(begCommand).c_str() : NULL, m_hashDatabasePath.c_str());
}

surface_ordering::iOrderingWrapper * CreateSurfaceViewerOrderingWrapper(central_pack_ptr pack, const wchar_t* basePath)
{
   return new OrderingWrapper(pack, basePath);
}