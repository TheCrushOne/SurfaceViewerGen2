#include "stdafx.h"
#include "FileStorageManager.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

namespace fs = std::filesystem;
using namespace file_storage;

void FileStorageManager::PrepareStorage(const wchar_t* metaFileName)
{
   return;
   std::unique_ptr<file_utils::global_path_storage> storage;// = GetPathStorageModify();
   storage->SetRouteElement(metaFileName);
   auto fname = SVGUtils::wstringToString(metaFileName);
   std::ifstream i(fname.c_str(), std::ios_base::in | std::ios::binary);
   if (!i.is_open())
      return;
   json j;
   i >> j;

   if (!j["log_rel_path"].empty())
      storage->logger_folder_path = storage->scenario_path + L"//" + SVGUtils::stringToWstring(j["log_rel_path"].get<std::string>());
   fs::path path(storage->logger_folder_path);
   if (!fs::exists(path))
      fs::create_directories(path);
}

file_storage::iFileStorageManager* CreateFileStorageManager(central_pack* pack)
{
   return new FileStorageManager(pack);
}