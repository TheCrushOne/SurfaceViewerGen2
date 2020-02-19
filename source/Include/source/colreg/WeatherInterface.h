#pragma once
#include "CommonStructs.h"
#include "CommonRefs.h"
#include <vector>

namespace colreg
{
#pragma pack (push, 4)

   //! сектор углов, относительных курсу корабля, запрещенный для поворота(0-359). Направление по часовой стрелки
   struct forbidden_turn_sector
   {
      forbidden_turn_sector() = default;
      forbidden_turn_sector(double af, double at) 
         : angleFrom{ af }
         , angleTo{ at } 
      {}
      double angleFrom = NO_VALUE;  //< начальный угол сектора, градусы
      double angleTo = NO_VALUE;    //< конечный угол сектора, градусы
   };

   //! информация о погоде
   struct weather_info
   {
      double windDirection = 0.;     // направление ветра (направление откуда дует) [градусы]
      double windSpeed = 0.;         // скорость ветра [м/с]
      double currentDirection = 0.;  // направление течения (направление куда течет) [градусы]
      double currentSpeed = 0.;      // скорость течения [м/с]
      double waveHeight = 0.;        // высота ветровой волны - wind wave significant 1/3 height [м]
      double waveDirection = 0.;     // направление ветровой волны (направление откуда идет) [градусы]
      double swellLength = 0.;       // длина зыби [м]
      double swellHeight = 0.;       // высота зыби [м]
      double swellDirection = 0.;    // направление зыби (направление откуда идет) [градусы]
      //bool isFog = false;            // туман
   };

   // Нормализация направления 360
   inline double normal_360(double K) { K -= (int)(K / 360) * 360.; return K < 0 ? 360. + K : K; }

   inline weather_info normalize(const weather_info& w, const track_point_info& move_info)
   {
      weather_info res = w;
      res.windDirection    = normal_360(res.windDirection - move_info.course);
      res.currentDirection = normal_360(res.currentDirection - move_info.course);
      res.waveDirection    = normal_360(res.waveDirection - move_info.course);
      res.swellDirection   = normal_360(res.swellDirection - move_info.course);
      return res;
   }

   using ForbiddenTurnSectors = std::vector<forbidden_turn_sector>;

   //! нормированный список секторов
   inline ForbiddenTurnSectors process_forbidden_turn_sector(const forbidden_turn_sector& fts)
   {
      ForbiddenTurnSectors res;
      if (fts.angleFrom == colreg::NO_VALUE || fts.angleTo == colreg::NO_VALUE)
         return res;

      const auto start = normal_360(fts.angleFrom);
      const auto end   = normal_360(fts.angleTo);
      if (start > end)
      {
         res.emplace_back(forbidden_turn_sector{ start, 360. });
         res.emplace_back(forbidden_turn_sector{ 0., end });
      }
      else
         res.emplace_back(forbidden_turn_sector{ start, end });

      return res;
   }

   struct weather_influence_info
   {
      weather_info            weather;
      forbidden_turn_sector   fts;        //< сектор углов запрещенных для поворота (на основе штормовых диаграм и тп)
   };

#pragma pack (pop)

   //! Интерфейс для учета влияния погоды
   struct iWeatherInfluence : iReleasable
   {
      /*! Оценить влияние погоды на корабль
      \param[in] info параметры корабля
      \param[in] move_info позиция + параметры движения корабля
      */
      virtual weather_info GetWeatherInfo(const geo_point& pos) const = 0;
      virtual forbidden_turn_sector Estimate(const ship_info& info, const track_point_info& move_info) const = 0;
      virtual bool IsFog(const geo_point& pos) const = 0;
   };

}