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

using namespace SV;
using namespace SV::surface_ordering;

OrderingWrapper::OrderingWrapper(central_pack* pack, const char* databasePath)
   : Central(pack)
   , m_hashDatabasePath(databasePath)
{
   m_cacheFolder = std::filesystem::absolute(std::filesystem::current_path().generic_string() + "\\..\\..\\..\\cache\\").generic_string();
   m_orderCacheFolder = m_cacheFolder + "order_heap\\";
   m_orderHeapFolder = std::filesystem::absolute(std::filesystem::current_path().generic_string() + "\\..\\..\\..\\order_heap\\").generic_string();

   VALID_CHECK_DLL_LOAD("NavigationDispatcher", "CreateNavigationDispatcher", m_navigationDispatcher, pack);
}

void OrderingWrapper::prepareCommandFromTemplate(std::string sourcePath, std::string dstPath, std::unordered_map<std::string, std::string> dict)
{
   std::filesystem::path dPath(dstPath);
   std::filesystem::create_directories(dPath.parent_path());
   std::filesystem::copy_file(sourcePath, dstPath, std::filesystem::copy_options::overwrite_existing);
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
      std::string token = "\\$\\(" + dictPair.first + "\\)";
      replace(fileData, token, dictPair.second);
   }
   file.close();

   std::ofstream ofile(dstPath, std::ios::out);
   ofile << fileData;
}

bool OrderingWrapper::ProcessOrder(const char* orderFileName, const char* begCommand, std::unordered_map<std::string, std::string>& dict)
{
   std::string srcOrderPath = m_orderHeapFolder + orderFileName;
   std::string dstOrderPath = m_orderCacheFolder + orderFileName;
   prepareCommandFromTemplate(srcOrderPath, dstOrderPath, dict);
   return m_navigationDispatcher->ProcessCommand(
      dstOrderPath.c_str(),
      begCommand ? begCommand : NULL,
      m_hashDatabasePath.c_str(),
      nullptr
   );
}

iOrderingWrapper * CreateSurfaceViewerOrderingWrapper(central_pack* pack, const char* basePath)
{
   return new OrderingWrapper(pack, basePath);
}