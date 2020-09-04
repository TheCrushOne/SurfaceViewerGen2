#pragma once

#include <string>

namespace file_utils
{
   inline void replaceExtension(std::string& data, const char* base, const char* nw)
   {
      data.replace(data.find(base), data.length(), nw);
   }

   inline void replaceExtension(std::wstring& data, const wchar_t* base, const wchar_t* nw)
   {
      data.replace(data.find(base), data.length(), nw);
   }

   inline void replaceTailFile(std::wstring& data)
   {
      data.replace(data.find_last_of(L"\\\\"), data.length(), L"");
      //data.replace(data.find_last_of(L"//"), sizeof(data) - 1, nw);
   }

   inline std::wstring getFileName(const std::wstring& data)
   {
      auto result = data;
      result.replace(data.find_last_of(L"."), data.length(), L"");
      result.replace(0, data.find_last_of(L"\\\\"), L"");
      return result;
   }

   struct global_path_storage
   {
      // base
      std::wstring meta_path;
      std::wstring scenario_path;

      // heightmap
      std::wstring pathfinder_settings_path;
      std::wstring research_settings_path;
      std::wstring environment_settings_path;
      std::wstring map_settings_path;
      std::wstring simulation_settings_path;
      std::wstring unit_data_path;
      std::wstring map_path;

      // database common
      std::wstring coordinate_map_path;

      // xml database special
      std::wstring map_object_path;

      // sqlite database special
      std::wstring database_path;

      // logger
      std::wstring logger_folder_path;

      global_path_storage() {}

      global_path_storage(std::wstring metaFileName)
      {
         SetRouteElement(metaFileName);
      }

      void SetRouteElement(std::wstring metaFileName)
      {
         meta_path = metaFileName;
         scenario_path = metaFileName;
         pathfinder_settings_path = metaFileName;
         research_settings_path = metaFileName;
         environment_settings_path = metaFileName;
         map_settings_path = metaFileName;
         simulation_settings_path = metaFileName;
         map_path = metaFileName;
         unit_data_path = metaFileName;
         coordinate_map_path = metaFileName;
         map_object_path = metaFileName;
         database_path = metaFileName;
         logger_folder_path = L"";

         replaceExtension(pathfinder_settings_path, L".meta", L".ps");
         replaceExtension(research_settings_path, L".meta", L".rs");
         replaceExtension(environment_settings_path, L".meta", L".es");
         replaceExtension(map_settings_path, L".meta", L".ms");
         replaceExtension(simulation_settings_path, L".meta", L".ss");
         replaceExtension(map_path, L".meta", L".png");
         replaceExtension(unit_data_path, L".meta", L".udp");

         replaceExtension(coordinate_map_path, L".meta", L".dat");
         replaceExtension(map_object_path, L".meta", L".xml");

         replaceExtension(database_path, L".meta", L".db3");

         replaceTailFile(scenario_path);
      }
   };
}