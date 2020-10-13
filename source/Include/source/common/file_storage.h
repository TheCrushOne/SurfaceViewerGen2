#pragma once

#include <string>
#include <functional>

namespace file_utils
{
   inline std::string replaceExtension(const std::string& data, const char* base, const char* nw)
   {
      std::string wdata = data;
      wdata.replace(wdata.find(base), wdata.length(), nw);
      return wdata;
   }

   inline std::wstring replaceExtension(const std::wstring& data, const wchar_t* base, const wchar_t* nw)
   {
      std::wstring wdata = data;
      wdata.replace(wdata.find(base), wdata.length(), nw);
      return wdata;
   }

   inline std::wstring replaceTailFile(const std::wstring& data)
   {
      std::wstring wdata = data;
      wdata.replace(wdata.find_last_of(L"\\\\"), wdata.length(), L"");
      return wdata;
      //data.replace(data.find_last_of(L"//"), sizeof(data) - 1, nw);
   }

   inline std::wstring getFileName(const std::wstring& data)
   {
      std::wstring result = data;
      result.replace(result.find_last_of(L"."), result.length(), L"");
      result.replace(0, result.find_last_of(L"\\\\"), L"");
      return result;
   }

   typedef std::function<std::wstring(const std::wstring&)> rapida_filename_translator;

   struct path_storage_unit
   {
      std::wstring base;
      std::wstring path;

      rapida_filename_translator translator;

      void mount() { path = translator(base); }
      void mount(const std::wstring& rpath) { path = rpath; }
      void condition_mount(const std::wstring& rpath, bool cond) { path = cond ? rpath : translator(base); }

      path_storage_unit(const path_storage_unit& ps)
         : base(ps.base)
         , path(ps.path)
         , translator(ps.translator)
      {}
      path_storage_unit() {}
      path_storage_unit(const std::wstring& base_path, const rapida_filename_translator& base_translator)
         : base(base_path)
         , translator(base_translator)
      {
         mount();
      }
   };

   struct global_path_storage
   {
      // base
      std::wstring meta_file_name;
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

      global_path_storage(std::wstring metaFileName)
      {
         set_route_element(metaFileName);
      }

      void set_route_element(std::wstring metaFileName)
      {
         meta_file_name = metaFileName;
         meta_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return base; });
         scenario_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return replaceTailFile(base); });
         pathfinder_settings_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return replaceExtension(base, L".meta", L".ps"); });
         research_settings_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return replaceExtension(base, L".meta", L".rs"); } );
         environment_settings_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return replaceExtension(base, L".meta", L".es"); });
         map_settings_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return replaceExtension(base, L".meta", L".ms"); });
         simulation_settings_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return replaceExtension(base, L".meta", L".ss"); });
         map_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return replaceExtension(base, L".meta", L".png"); });
         unit_data_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return replaceExtension(base, L".meta", L".udp"); });
         coordinate_map_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return replaceExtension(base, L".meta", L".dat"); });
         map_object_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return replaceExtension(base, L".meta", L".png"); });
         database_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return replaceExtension(base, L".meta", L".db3"); });
         logger_folder_path = path_storage_unit(meta_file_name, [](const std::wstring& base)->std::wstring { return base; });
      }
   };
}