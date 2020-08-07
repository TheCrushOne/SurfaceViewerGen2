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

   struct chart_object_gen_order
   {
      _bstr_t destination;
      _bstr_t source;
   };

   struct png_hm_convert_order
   {
      _bstr_t destination;
      _bstr_t source;
   };

   struct pathfind_order
   {
      _bstr_t destination;
      _bstr_t source;
   };

   struct opt_pathfind_order
   {
      _bstr_t destination;
      _bstr_t source;
   };

   struct packhound_order
   {
      _bstr_t destination;
      _bstr_t source;
   };
}