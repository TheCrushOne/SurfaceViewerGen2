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
      std::string unit_data;
      std::string env_stt;
      std::string map_stt;
      std::string pth_stt;
      std::string res_stt;
      std::string sim_stt;

      struct tag
      {
         static constexpr char folder[] = "folder";
         static constexpr char unit_data[] = "unit_data";
         static constexpr char env_stt[] = "env_stt";
         static constexpr char map_stt[] = "map_stt";
         static constexpr char pth_stt[] = "pth_stt";
         static constexpr char res_stt[] = "res_stt";
         static constexpr char sim_stt[] = "sim_stt";
      };

      bool Deserialize(const xml_properties::PropertyItem& standart) override
      {
         standart[tag::folder].Get(folder);
         standart[tag::unit_data].Get(unit_data);
         if (standart.Exists(tag::env_stt)) standart[tag::env_stt].Get(env_stt);
         if (standart.Exists(tag::map_stt)) standart[tag::map_stt].Get(map_stt);
         if (standart.Exists(tag::pth_stt)) standart[tag::pth_stt].Get(pth_stt);
         if (standart.Exists(tag::res_stt)) standart[tag::res_stt].Get(res_stt);
         if (standart.Exists(tag::sim_stt)) standart[tag::sim_stt].Get(sim_stt);
         return true;
      }
   };
}