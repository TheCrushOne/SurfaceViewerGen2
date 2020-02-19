#pragma once

#include <limits>
#include <string>
#include <vector>
#include "coordinates.h"
#include "SVCG/route_point.h"

namespace settings
{
   struct point_setting_element
   {
      // TODO: заменить комбинеды на что-нибудь нормальное
      std::string name;
      SVCG::route_point start;
      SVCG::route_point finish;
      std::vector<SVCG::route_point> control_point_list;
   };

   struct unit_settings
   {
      std::vector<point_setting_element> land_units;
      std::vector<point_setting_element> air_units;
   };

   struct point_settings
   {
      std::string settings_rel_path;
   };

   struct level_settings
   {
      double max_air_height;
      double max_land_height;
      double min_land_height;
      double max_land_angle;
      double dangerous_land_angle;
      level_settings()
         : max_air_height(0.f)
         , max_land_height(0.f)
         , min_land_height(0.f)
         , max_land_angle(0.f)
         , dangerous_land_angle(0.f)
      {}
   };

    struct pathfinding_settings
    {
       point_settings point_settings;
       level_settings level_settings;
    };

    template<typename T>
    struct range_data
    {
        T min;
        T max;
        T step;
    };

    struct research_settings
    {
        range_data<size_t> count_range;
        range_data<double> length_range;
        size_t iter_count;
        size_t map_size;
        bool multi_thread_test;
        bool single_thread_test;
        size_t debug_level;   // TODO: подкорректировать
    };

    struct application_settings
    {
        pathfinding_settings pth_stt;
        research_settings res_stt;
        unit_settings unit_stt;

        application_settings() = default;

        application_settings(const pathfinding_settings& pthStt, const research_settings& resStt, const unit_settings& unitStt)
            : pth_stt(pthStt)
            , res_stt(resStt)
            , unit_stt(unitStt)
        {}
    };
}
