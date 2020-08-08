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

   struct order_base
   {
      //std::string name;

      virtual bool Deserialize(const xml_properties::PropertyItem&) = 0;
   };

   struct chart_object_gen_order : order_base
   {
      std::string source;
      std::string destination;

      struct tag
      {
         static constexpr char source[] = "source";
         static constexpr char destination[] = "destination";
      };

      virtual bool Deserialize(const xml_properties::PropertyItem& params)
      {
         params[tag::source].Get(source);
         params[tag::destination].Get(destination);
         return true;
      }
   };

   struct png_hm_convert_order : order_base
   {
      std::string destination;
      std::string source;

      struct tag
      {
         static constexpr char source[] = "source";
         static constexpr char destination[] = "destination";
      };

      virtual bool Deserialize(const xml_properties::PropertyItem& params)
      {
         params[tag::source].Get(source);
         params[tag::destination].Get(destination);
         return true;
      }
   };

   struct pathfind_order : order_base
   {
      std::string destination;
      std::string source;

      struct tag
      {
         static constexpr char source[] = "source";
         static constexpr char destination[] = "destination";
      };

      virtual bool Deserialize(const xml_properties::PropertyItem& params)
      {
         params[tag::source].Get(source);
         params[tag::destination].Get(destination);
         return true;
      }
   };

   struct opt_pathfind_order : order_base
   {
      std::string destination;
      std::string source;

      struct tag
      {
         static constexpr char source[] = "source";
         static constexpr char destination[] = "destination";
      };

      virtual bool Deserialize(const xml_properties::PropertyItem& params)
      {
         params[tag::source].Get(source);
         params[tag::destination].Get(destination);
         return true;
      }
   };

   struct packhound_order : order_base
   {
      std::string destination;

      struct tag
      {
         static constexpr char destination[] = "destination";
      };

      virtual bool Deserialize(const xml_properties::PropertyItem& params)
      {
         params[tag::destination].Get(destination);
         return true;
      }
   };
}