#pragma once

#include <limits>
#include <string>
#include <vector>
#include "coordinates.h"
#include "SVCG/route_point.h"

#include "settings_types.h"
#include "settings_meta.h"

namespace settings
{
   struct route
   {
      SVCG::route_point start;
      SVCG::route_point finish;
      std::vector<SVCG::route_point> control_point_list;
      std::vector<SVCG::route_point> route_list;
      route() {}
      route(SVCG::route_point& start, SVCG::route_point& finish)
         : start(start)
         , finish(finish)
         , control_point_list()
         , route_list()
      {}
      route(SVCG::route_point& start, SVCG::route_point& finish, std::vector<SVCG::route_point>& route, std::vector<SVCG::route_point>& controlPointList)
         : start(start)
         , finish(finish)
         , control_point_list(controlPointList)
         , route_list(route)
      {}
      route(SVCG::route_point& start, SVCG::route_point& finish, std::vector<SVCG::route_point>& controlPointList)
         : start(start)
         , finish(finish)
         , control_point_list(controlPointList)
         , route_list()
      {}
   };

   struct unit_data_element : public route
   {
      std::string name;
      unit_data_element()
         : route()
      {}
      unit_data_element(std::string name)
         : route()
         , name(name)
      {}
      unit_data_element(std::string name, SVCG::route_point& start, SVCG::route_point& finish)
         : route(start, finish)
         , name(name)
      {}
      unit_data_element(std::string name, SVCG::route_point& start, SVCG::route_point& finish, std::vector<SVCG::route_point>& route, std::vector<SVCG::route_point>& controlPointList)
         : route(start, finish, route, controlPointList)
         , name(name)
      {}
      unit_data_element(std::string name, SVCG::route_point& start, SVCG::route_point& finish, std::vector<SVCG::route_point>& controlPointList)
         : route(start, finish, controlPointList)
         , name(name)
      {}
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

   // NOTE: min, max, step формируют наборные значени€, если вектор пустой в исходных
   template<typename T>
   struct range_data
   {
      T min;
      T max;
      T step;
      std::vector<T> values;
   };

   struct research_settings
   {
      ResearchType res_type;
      range_data<size_t> thread_pool_range; // количество активных потоков
      range_data<size_t> task_pool_range; // количество задач в пуле расчета
      range_data<size_t> fly_count_range;  // количество летающих
      range_data<double> length_range; // длина пути
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
      double angle = 0.; // угол между x и ординатой этой — 
      double scale = 1.; // относительный масштаб(т.е. 1к[scale])
      double ordinate_bias;  // смещение по ординате
      double abscissa_bias;  // смещение по абсциссе
   };

   struct environment_settings
   {
      // Ќастройки сделаны относительно пикселей,
      // т.к. экран прив€зан именно к ним
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
