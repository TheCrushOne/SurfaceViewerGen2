#pragma once

#include <vector>
#include <functional>
#include <memory>
#include "pathfinder_types.h"
#include "SVCG/route_point.h"
#include "SVCG/geo_point.h"
#include "common/header_collector.h"
#include "common/matrix_object.h"

namespace SV::pathfinder
{
   struct check_fly_zone_result
   {
      FlyZoneAffilation fza;
   };

   struct check_go_zone_result
   {
      GoZoneAffilation gza;
      double asn; // angle south-north
      double awe; // angle west-east
      double aswne;  // angle southwest-northeast
      double asenw;  // angle southeast-northwest
   };

   struct strategy_settings
   {
      StrategyType type;
      double radius;
   };

   struct route_data
   {
      std::vector<settings::route> land_routes;
      std::vector<settings::route> air_routes;
      route_data() {}
      route_data(std::vector<settings::route>& landRoutes, std::vector<settings::route>& airRoutes)
         : land_routes(landRoutes)
         , air_routes(airRoutes)
      {}
   };

   using aff_checker = std::function<bool(const SharedRoutePointMatrix&, const SharedUnsignedMatrix&, size_t, size_t)>;

   struct path_finder_logic
   {
      aff_checker checker;
      height_corrector corrector;
      path_finder_logic(aff_checker checker, height_corrector corrector)
         : checker(checker)
         , corrector(corrector)
      {}

      const aff_checker& GetChecker() const { return checker; }
      const height_corrector& GetCorrector() const { return corrector; }
   };

   struct path_finder_task
   {
      size_t unit_index;
      size_t shard_index;
      CG::route_point start;
      CG::route_point finish;
      path_finder_logic logic;
      SharedRoutePointMatrix rawdata;
      SharedUnsignedMatrix coverageMatrix;

      std::function<void(void)> runnable;

      CG::route_line path;
      bool path_found = false;

      bool counted = false;

      path_finder_task(aff_checker checker, height_corrector corrector)
         : logic(checker, corrector)
      {}
   };

   // А это рантайм? или че? а то дублируются...
   // Настройки патфайндера...и в принципе всё..
   struct path_finder_settings
   {
      bool multithread;
      // NOTE: Это для статистики
      std::vector<size_t> stat_field_index;    // Статистическое поле - н-мерная матрица статистики

      bool use_strategies; // флаг использования стратегий(для исследований)
      bool research;       // флаг исследовательского пробега
      bool land_path;      // необходимость расчета наземного пути
      size_t packet_size;  // размер пакета путей для многопоточного вызова(0 - все задачи в 1 пул)
      size_t thread_count; // количество запрашиваемых потоков

      path_finder_settings(bool multithread = false, std::vector<size_t> statFieldIndex = {}, bool research = false, bool landPath = true, size_t packetSize = 2, size_t threadCount = 8, bool useStrategies = true)
         : multithread(multithread)
         , stat_field_index(statFieldIndex)
         , research(research)
         , land_path(landPath)
         , packet_size(packetSize)
         , thread_count(threadCount)
         , use_strategies(useStrategies)
      {}
   };

   // Статистика для вывода в файл
   struct path_finder_statistic
   {
      // Пока что пусто
   };

   struct path_finder_indata
   {
      settings::unit_source_data unit_data;
      path_finder_settings settings;
      path_finder_statistic statistic;
      strategy_settings strategy_settings;
   };
}