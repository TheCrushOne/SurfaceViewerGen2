#pragma once

#include <string>

namespace file_utils
{
   inline void replaceExtension(std::wstring& data, const wchar_t* base, const wchar_t* nw)
   {
      data.replace(data.find(base), sizeof(data) - 1, nw);
   }

   struct file_storage_base
   {
      std::wstring meta_path;

      file_storage_base(std::wstring metaFileName)
         : meta_path(metaFileName)
      {}
   };

   struct heightmap_file_storage : file_storage_base
   {
      std::wstring pathfinder_settings_path;
      std::wstring research_settings_path;
      std::wstring unit_data_path;
      std::wstring map_path;

      heightmap_file_storage(std::wstring metaFileName)
         : file_storage_base(metaFileName)
         , pathfinder_settings_path(metaFileName)
         , research_settings_path(metaFileName)
         , map_path(metaFileName)
         , unit_data_path(metaFileName)
      {
         replaceExtension(pathfinder_settings_path, L".meta", L".psxml");
         replaceExtension(research_settings_path, L".meta", L".rsxml");
         replaceExtension(map_path, L".meta", L".png");
         replaceExtension(unit_data_path, L".meta", L".udp");
      }
   };

   struct xml_database_file_storage : file_storage_base
   {
      std::wstring pathfinder_settings_path;
      std::wstring research_settings_path;
      std::wstring unit_data_path;
      std::wstring coordinate_map_path;
      std::wstring map_object_path;
      
      xml_database_file_storage(std::wstring metaFileName)
         : file_storage_base(metaFileName)
         , pathfinder_settings_path(metaFileName)
         , research_settings_path(metaFileName)
         , unit_data_path(metaFileName)
         , coordinate_map_path(metaFileName)
         , map_object_path(metaFileName)
      {
         replaceExtension(pathfinder_settings_path, L".meta", L".psxml");
         replaceExtension(research_settings_path, L".meta", L".rsxml");
         replaceExtension(unit_data_path, L".meta", L".udp");
         replaceExtension(coordinate_map_path, L".meta", L".dat");
         replaceExtension(map_object_path, L".meta", L".xml");
      }
   };

   struct sqlite_database_file_storage : file_storage_base
   {
      //std::wstring pathfinder_settings_path;
      //std::wstring research_settings_path;
      //std::wstring unit_data_path;
      std::wstring coordinate_map_path;
      std::wstring database_path;
      //std::wstring map_object_path;

      sqlite_database_file_storage(std::wstring metaFileName)
         : file_storage_base(metaFileName)
         , database_path(metaFileName)
         //, pathfinder_settings_path(metaFileName)
         //, research_settings_path(metaFileName)
         //, unit_data_path(metaFileName)
         , coordinate_map_path(metaFileName)
         //, map_object_path(metaFileName)
      {
         //replaceExtension(pathfinder_settings_path, L".meta", L".psxml");
         //replaceExtension(research_settings_path, L".meta", L".rsxml");
         //replaceExtension(unit_data_path, L".meta", L".udp");
         replaceExtension(coordinate_map_path, L".meta", L".dat");
         replaceExtension(database_path, L".meta", L".sq3");
         //replaceExtension(map_object_path, L".meta", L".ms3");
      }
   };
}