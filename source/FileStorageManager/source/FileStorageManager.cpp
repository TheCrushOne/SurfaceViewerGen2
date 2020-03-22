#include "stdafx.h"
#include "FileStorageManager.h"
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using namespace nlohmann;
using namespace file_storage;

file_storage::iFileStorageManager* CreateFileStorageManager()
{
   return new FileStorageManager();
}

void FileStorageManager::PrepareStorage(const wchar_t* metaFileName)
{
   auto& storage = GetPathStorageModify();
   auto fname = SVGUtils::wstringToString(metaFileName);
   std::stringstream i(fname.c_str(), std::ios_base::in | std::ios::binary);
   json j;
   i >> j;

   storage.logger_folder_path = storage.scenario_path + L"//" + j["log_rel_path"].get<std::wstring>();
   fs::path path(storage.logger_folder_path);
   if (!fs::exists(path))
      fs::create_directories(path);
}