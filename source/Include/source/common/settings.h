#pragma once

#include <limits>
#include <string>
#include <vector>
#include "coordinates.h"
#include "SVCG/route_point.h"

#include "settings_meta.h"

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

    struct simulation_settings
    {};

    struct coordinate_system_info
    {
       double angle = 0.; // угол между x и ординатой этой СК
       double scale = 1.; // относительный масштаб(т.е. 1к[scale])
       double ordinate_bias;  // смещение по ординате
       double abscissa_bias;  // смещение по абсциссе
    };

    struct environment_settings
    {
       // Настройки сделаны относительно пикселей,
       // т.к. экран привязан именно к ним
       coordinate_system_info gcs_info;   // geographical coordinate system
       coordinate_system_info mtx_info;   // matrix coordinate system
    };

    struct application_settings
    {
        pathfinding_settings pth_stt;
        research_settings res_stt;
        unit_settings unit_stt;
        environment_settings env_stt;
        simulation_settings sim_stt;

        application_settings() = default;

        application_settings(const pathfinding_settings& pthStt, const research_settings& resStt, const unit_settings& unitStt)
            : pth_stt(pthStt)
            , res_stt(resStt)
            , unit_stt(unitStt)
        {}
    };    
}
