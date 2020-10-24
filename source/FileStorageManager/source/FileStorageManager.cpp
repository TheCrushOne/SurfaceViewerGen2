#include "stdafx.h"
#include "FileStorageManager.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

#include "json/json_wrapper_impl.h"

namespace fs = std::filesystem;
using namespace SV;
using namespace SV::file_storage;

void FileStorageManager::PrepareStorage(const wchar_t* metaFileName)
{
   return;
   std::unique_ptr<file_utils::global_path_storage> storage;// = GetPathStorageModify();
   //storage->SetRouteElement(metaFileName);
   auto fname = SVGUtils::wstringToString(metaFileName);
   std::ifstream i(fname.c_str(), std::ios_base::in | std::ios::binary);
   if (!i.is_open())
      return;
   Json::Value j;
   i >> j;

   if (!j[tag::log_rel_path].empty())
      storage->logger_folder_path.path = storage->scenario_path.path + L"//" + SVGUtils::stringToWstring(j[tag::log_rel_path].asString());
   fs::path path(storage->logger_folder_path.path);
   if (!fs::exists(path))
      fs::create_directories(path);
}

file_storage::iFileStorageManager* CreateFileStorageManager(central_pack* pack)
{
   return new FileStorageManager(pack);
}