#pragma once

#include "DataStandartTypes.h"
#include <comutil.h>

//#define DEFINE_DATASOURCE_TYPE( ds_type ) enum { data_source_type = (ds_type) };

namespace data_standart
{
   struct data_standart_base
   {
      // NOTE: мастхэв параметр, ибо мусора настакаться может много
      bool clear_dst = true;

      virtual bool Deserialize(const xml_properties::PropertyItem&) = 0;
   };

   struct png_hm_data_standart : data_standart_base
   {
      std::string file;

      struct tag
      {
         static constexpr char file[] = "file";
      };

      bool Deserialize(const xml_properties::PropertyItem& standart) override
      {
         standart[tag::file].Get(file);
         return true;
      }
   };

   struct chart_object_data_standart : data_standart_base
   {
      std::string folder;

      struct tag
      {
         static constexpr char folder[] = "folder";
      };

      bool Deserialize(const xml_properties::PropertyItem& standart) override
      {
         standart[tag::folder].Get(folder);
         return true;
      }
   };

   struct path_storage_data_standart : data_standart_base
   {
      std::string folder;

      struct tag
      {
         static constexpr char folder[] = "folder";
      };

      bool Deserialize(const xml_properties::PropertyItem& standart) override
      {
         standart[tag::folder].Get(folder);
         return true;
      }
   };

   struct optimized_path_storage_data_standart : data_standart_base
   {
      std::string folder;

      struct tag
      {
         static constexpr char folder[] = "folder";
      };

      bool Deserialize(const xml_properties::PropertyItem& standart) override
      {
         standart[tag::folder].Get(folder);
         return true;
      }
   };

   struct svgm_data_standart : data_standart_base
   {
      std::string folder;

      struct tag
      {
         static constexpr char folder[] = "folder";
      };

      bool Deserialize(const xml_properties::PropertyItem& standart) override
      {
         standart[tag::folder].Get(folder);
         return true;
      }
   };
}