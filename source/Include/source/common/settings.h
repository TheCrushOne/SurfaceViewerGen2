#pragma once

#include <limits>
#include <string>
#include <vector>
#include "coordinates.h"
#include "SVCG/route_point.h"

#include "settings_meta.h"

namespace settings
{
   struct unit_data_element
   {
      std::string name;
      SVCG::route_point start;
      SVCG::route_point finish;
      std::vector<SVCG::route_point> control_point_list;
      std::vector<SVCG::route_point> route_point_list;
   };

   struct unit_source_data
   {
      std::vector<unit_data_element> land_units;
      std::vector<unit_data_element> air_units;
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

    struct map_settings
    {
       size_t row_count;
       size_t col_count;
    };

    struct application_settings
    {
        pathfinding_settings pth_stt;
        research_settings res_stt;
        environment_settings env_stt;
        simulation_settings sim_stt;
        map_settings map_stt;

        application_settings() = default;

        application_settings(const pathfinding_settings& pthStt, const research_settings& resStt, const environment_settings& envStt, const simulation_settings& simStt, const map_settings& mapStt)
           : pth_stt(pthStt)
           , res_stt(resStt)
           , env_stt(envStt)
           , sim_stt(simStt)
           , map_stt(mapStt)
        {}
    };    
}
