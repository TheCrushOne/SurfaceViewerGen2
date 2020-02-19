#pragma once

#pragma pack(push, 1)


namespace colreg
{
/*
recomended weight
   enum class PenaltyCategorySeverety //categories and weights
   {
      PCS_LOWEST = 1,
      PCS_LOW = 5,
      PCS_BELOW_NORMAL = 15,
      PCS_NORMAL = 20,
      PCS_ABOVE_NORMAL = 25,
      PCS_HIGH = 40,
   };
   */

   //веса штрафов при расхождении встречными курсами
   struct gmp_heading_courses_maneuver_cfg
   {           //                                                                                                           ____
      double badHeadingMeneuver = 50.0;   // половина максимального для GMP. предложенный маневр не является трапецевидным  __/    \_
      double secondTurnPenalty = 40.0;    // вес штрафа - плохой поворот в точке P2 (цель должна быть в 4-ой четверти)
      double thirdTurnPenalty = 40.0;     // вес штрафа - плохой поворот в точке P3 (цель должна быть в 3-ой четверти, а лучше слева за кормой)
      double offsetPenalty = 40.0;        // вес штрафа за слишком большую дистанцию расхождения от линии текущего движения
      double maxParallelAngleDelta = 3.0; // макс допустимая разница курсов на пером и третьем сегментах маневра. в идеале они должны быть параллельны 
   };

   //веса штрафов при отвороте вправо
   struct gmp_crossing_right_cfg
   {           
      double minPenalty = 10.0;
      double turnToSternPenalty = 40.0;  //вес штрафа за поворот меньше чем на корму другого корабля
      double leaveBehindPenalty = 40.0;  //вес штрафа за НЕ оставления корабля за кормой (поворот обратно раньше чем он будет за кормой)
   };

   /*
   Структура для задания настроек учета хорошей морской практики
   */
   struct good_marine_practice_settings
   {
      bool checkTargetAlarm;           //< при расхождение проверять, что маневр не "пугает" остальные корабли
      bool checkCourseIntersection;    //< при расхождение проверять, что маневр не приводит к опасному пересечению курсов других кораблей
      double safeCouseIntersection;    //< CPA *  safeCouseIntersaction - безопасная дистанция пересечения курса другого корабля
      double maxPenalty;               //< максимальный штраф для маневров Хорошей Морской Практики (GMP)
      gmp_heading_courses_maneuver_cfg headingCoursesPenalties; //< настройка штрафов для GMP встречных курсов
      gmp_crossing_right_cfg           crossingRightPenalties;  //< настройка штрафов для отворота направо
      good_marine_practice_settings()
         : checkTargetAlarm{ true }
         , checkCourseIntersection{ true }
         , safeCouseIntersection{ 2. }
         , maxPenalty{100.0}
      {}
   };
}
#pragma pack(pop)