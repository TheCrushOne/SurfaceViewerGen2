#pragma once

#include <string>
#include <functional>

namespace SV::file_utils
{
   inline std::string replaceExtension(const std::string& data, const char* base, const char* nw)
   {
      std::string wdata = data;
      wdata.replace(wdata.find(base), wdata.length(), nw);
      return wdata;
   }

   // NOTE: запаска на случай вайд чаров
   inline std::wstring replaceExtension(const std::wstring& data, const wchar_t* base, const wchar_t* nw)
   {
      std::wstring wdata = data;
      wdata.replace(wdata.find(base), wdata.length(), nw);
      return wdata;
   }

   inline std::string replaceTailFile(const std::string& data)
   {
      std::string wdata = data;
      wdata.replace(wdata.find_last_of("\\\\"), wdata.length(), "");
      return wdata;
      //data.replace(data.find_last_of(L"//"), sizeof(data) - 1, nw);
   }

   inline std::string getFileName(const std::string& data)
   {
      std::string result = data;
      result.replace(result.find_last_of("."), result.length(), "");
      result.replace(0, result.find_last_of("\\\\"), "");
      return result;
   }

   typedef std::function<std::string(const std::string&)> rapida_filename_translator;

   struct path_storage_unit
   {
      std::string base;
      std::string path;

      rapida_filename_translator translator;

      void mount() { path = translator(base); }
      void mount(const std::string& rpath) { path = rpath; }
      void condition_mount(const std::string& rpath, bool cond) { path = cond ? rpath : translator(base); }

      path_storage_unit(const path_storage_unit& ps)
         : base(ps.base)
         , path(ps.path)
         , translator(ps.translator)
      {}
      path_storage_unit() {}
      path_storage_unit(const std::string& base_path, const rapida_filename_translator& base_translator)
         : base(base_path)
         , translator(base_translator)
      {
         mount();
      }
   };

   struct global_path_storage
   {
      // base
      std::string meta_file_name;
      path_storage_unit meta_path;
      path_storage_unit scenario_path;

      // heightmap
      path_storage_unit pathfinder_settings_path;
      path_storage_unit research_settings_path;
      path_storage_unit environment_settings_path;
      path_storage_unit map_settings_path;
      path_storage_unit simulation_settings_path;
      path_storage_unit unit_data_path;
      path_storage_unit map_path;

      // database common
      path_storage_unit coordinate_map_path;

      // xml database special
      path_storage_unit map_object_path;

      // sqlite database special
      path_storage_unit database_path;

      // logger
      path_storage_unit logger_folder_path;

      global_path_storage() {}

      global_path_storage(std::string metaFileName)
      {
         set_route_element(metaFileName);
      }

      void set_route_element(const std::string& metaFileName)
      {
         meta_file_name = metaFileName;
         meta_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return base; });
         scenario_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return replaceTailFile(base); });
         pathfinder_settings_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return replaceExtension(base, ".meta", ".ps"); });
         research_settings_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return replaceExtension(base, ".meta", ".rs"); } );
         environment_settings_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return replaceExtension(base, ".meta", ".es"); });
         map_settings_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return replaceExtension(base, ".meta", ".ms"); });
         simulation_settings_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return replaceExtension(base, ".meta", ".ss"); });
         map_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return replaceExtension(base, ".meta", ".png"); });
         unit_data_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return replaceExtension(base, ".meta", ".udp"); });
         coordinate_map_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return replaceExtension(base, ".meta", ".dat"); });
         map_object_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return replaceExtension(base, ".meta", ".png"); });
         database_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return replaceExtension(base, ".meta", ".db3"); });
         logger_folder_path = path_storage_unit(meta_file_name, [](const std::string& base)->std::string { return base; });
      }
   };
}