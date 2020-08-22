#pragma once

#include <comutil.h>
#include <thread>

namespace navigation_dispatcher
{
   struct command_loop
   {
      command_loop()
         : min_val(0)
         , max_val(0)
         , step(1)
         , split_by_process(false)
      {
      }

      _bstr_t param;
      int min_val;
      int max_val;
      int step;
      bool split_by_process;

      bool is_valid() const
      {
         return max_val > min_val;
      }

      size_t loop_length() const
      {
         return is_valid() ? (max_val - min_val + 1) / step : 0;
      }
   };

   struct command_launch_params
   {
      int max_threads = std::thread::hardware_concurrency();
      int split_process_num = 1;
      command_loop loop_params;

      size_t calc_process_num() const
      {
         return loop_params.is_valid() && loop_params.split_by_process ? loop_params.loop_length() : split_process_num;
      }
   };

   struct attr_data
   {
      std::string val;

      const char* AsString() { return val.c_str(); }
      int AsInteger() { return atoi(val.c_str()); }
      double AsDouble() { return atof(val.c_str()); }
      bool AsBoolean()
      {
         // TODO: вы€снить, почему так сложно...
         std::string buffer = val;
         std::transform(buffer.begin(), buffer.end(), buffer.begin(),
            [](unsigned char c) { return std::tolower(c); });
         return buffer.compare("true") == 0;
      }
   };

   struct tag_meta
   {
      const char* tag;
      attr_data& data;
   };

#define ATTR_DECLARE(attr) attr_data attr;
#define BEGIN_DEFINE_ZONE() virtual void fillTagList() override {
#define END_DEFINE_ZONE() }
#define ATTR_DEFINE(attr) meta_list.emplace_back(tag_meta{ #attr, attr });

   struct order_base
   {
      ATTR_DECLARE(rewrite_dst)
      std::vector<tag_meta> meta_list;

      bool Deserialize(const xml_properties::PropertyItem& params)
      {
         ATTR_DEFINE(rewrite_dst)
         fillTagList();
         for (auto& meta : meta_list)
         {
            if (params.Exists(meta.tag))
               params[meta.tag].Get(meta.data.val);
         }
         return true;
      }

      virtual void fillTagList() = 0;
   };

   struct chart_object_gen_order : order_base
   {
      ATTR_DECLARE(source);
      ATTR_DECLARE(destination);

      BEGIN_DEFINE_ZONE()
         ATTR_DEFINE(source);
         ATTR_DEFINE(destination);
      END_DEFINE_ZONE()
   };

   struct png_hm_convert_order : order_base
   {
      ATTR_DECLARE(source);
      ATTR_DECLARE(destination);

      BEGIN_DEFINE_ZONE()
         ATTR_DEFINE(source);
         ATTR_DEFINE(destination);
      END_DEFINE_ZONE()
   };

   struct pathfind_order : order_base
   {
      ATTR_DECLARE(source);
      ATTR_DECLARE(destination);

      BEGIN_DEFINE_ZONE()
         ATTR_DEFINE(source);
         ATTR_DEFINE(destination);
      END_DEFINE_ZONE()
   };

   struct opt_pathfind_order : order_base
   {
      ATTR_DECLARE(source);
      ATTR_DECLARE(destination);

      BEGIN_DEFINE_ZONE()
         ATTR_DEFINE(source);
         ATTR_DEFINE(destination);
      END_DEFINE_ZONE()
   };

   struct packhound_order : order_base
   {
      ATTR_DECLARE(destination);

      BEGIN_DEFINE_ZONE()
         ATTR_DEFINE(destination);
      END_DEFINE_ZONE()
   };

#undef ATTR_DECLARE
#undef BEGIN_DEFINE_ZONE
#undef END_DEFINE_ZONE
#undef ATTR_DEFINE
}