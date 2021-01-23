#pragma once

#include <limits>
#include <string>
#include <vector>
#include "common/coordinates.h"
#include "SVCG/route_point.h"

#include "settings_types.h"
#include "settings_meta.h"

namespace SV::settings
{
   struct route
   {
      CG::route_point start;
      CG::route_point finish;
      CG::route_line control_point_list;
      CG::route_line route_list;

      route() {}
      route(const CG::route_point& start, const CG::route_point& finish)
         : start(start)
         , finish(finish)
         , control_point_list()
         , route_list()
      {}
      route(const CG::route_point& start, const CG::route_point& finish, const CG::route_line& route, const CG::route_line& controlPointList)
         : start(start)
         , finish(finish)
         , control_point_list(controlPointList)
         , route_list(route)
      {}
      route(const CG::route_point& start, const CG::route_point& finish, const CG::route_line& controlPointList)
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
      unit_data_element(const char* name)
         : route()
         , name(name)
      {}
      unit_data_element(const char* name, const CG::route_point& start, const CG::route_point& finish)
         : route(start, finish)
         , name(name)
      {}
      unit_data_element(const char* name, const CG::route_point& start, const CG::route_point& finish, const CG::route_line& route, const CG::route_line& controlPointList)
         : route(start, finish, route, controlPointList)
         , name(name)
      {}
      unit_data_element(const char* name, const CG::route_point& start, const CG::route_point& finish, const CG::route_line& controlPointList)
         : route(start, finish, controlPointList)
         , name(name)
      {}
   };

   struct unit_source_data
   {
      std::vector<unit_data_element> land_units;
      std::vector<unit_data_element> air_units;

      unit_source_data()
         : land_units({})
         , air_units({})
      {}
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
      level_settings lvl_stt;
      bool use_multithread_computing;

      pathfinding_settings()
         : lvl_stt(level_settings())
         , use_multithread_computing(false)
      {}
   };

   // NOTE: min, max, step формируют наборные значения, если вектор пустой в исходных
   template<typename T>
   struct range_data
   {
      T min_val;
      T max_val;
      T step;
      std::vector<T> values;

      range_data()
         : min_val(static_cast<T>(0))
         , max_val(static_cast<T>(0))
         , step(static_cast<T>(0))
         , values(std::vector<T>{})
      {}

      // TODO: сделать безопасность!!!
      void apply()
      {
         if (!values.empty() || step == 0)
            return;
         T cur = min_val;
         while (cur <= max_val)
         {
            values.emplace_back(cur);
            cur += step;
         }
      }
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
      size_t split_factor;
      bool multi_thread_test;
      bool single_thread_test;
      size_t debug_level;   // TODO: подкорректировать

      research_settings()
         : res_type(ResearchType::RT_TIME)
         , iter_count(0)
         , map_size(0)
         , split_factor(1) // NOTE: бьется на 1, т.е. не бьется
         , multi_thread_test(false)
         , single_thread_test(false)
         , debug_level(0)
      {}
   };

   struct simulation_settings
   {};

   struct coordinate_system_info
   {
      double angle = 0.; // угол между x и ординатой этой СК
      double scale = 1.; // относительный масштаб(т.е. 1к[scale])
      double ordinate_bias = 0.;  // смещение по ординате
      double abscissa_bias = 0.;  // смещение по абсциссе

      coordinate_system_info()
         : angle(0.)
         , scale(0.)
         , ordinate_bias(0.)
         , abscissa_bias(0.)
      {}
   };

   struct environment_settings
   {
      // Настройки сделаны относительно пикселей,
      // т.к. экран привязан именно к ним
      coordinate_system_info gcs_info;   // geographical coordinate system
      coordinate_system_info mtx_info;   // matrix coordinate system

      environment_settings()
         : gcs_info(coordinate_system_info())
         , mtx_info(coordinate_system_info())
      {}
   };

   struct map_settings
   {
      size_t row_count = 0;
      size_t col_count = 0;

      map_settings()
         : row_count(0)
         , col_count(0)
      {}
   };

   struct application_settings
   {
      pathfinding_settings pth_stt;
      research_settings res_stt;
      environment_settings env_stt;
      simulation_settings sim_stt;
      map_settings map_stt;

      application_settings()
         : pth_stt(pathfinding_settings())
         , res_stt(research_settings())
         , env_stt(environment_settings())
         , sim_stt(simulation_settings())
         , map_stt(map_settings())
      {}

      application_settings(const pathfinding_settings& pthStt, const research_settings& resStt, const environment_settings& envStt, const simulation_settings& simStt, const map_settings& mapStt)
         : pth_stt(pthStt)
         , res_stt(resStt)
         , env_stt(envStt)
         , sim_stt(simStt)
         , map_stt(mapStt)
      {}
   };
}
