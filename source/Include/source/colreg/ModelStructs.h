#pragma once

namespace colreg
{
   //! Режим движения модели
   enum class MODEL_MOVE_MODE : int
   {
      MMM_FORWARD = 0,        ///< По прямой с постоянной скоростью
      MMM_CIRCULATION_LEFT,   ///< Циркуляция влево
      MMM_CIRCULATION_RIGHT,  ///< Циркуляция вправо
      MMM_ACCSELERATION_UP,   ///< Ускорение
      MMM_ACCSELERATION_DOWN, ///< Торможение
   };

#pragma pack (push, 1)

   struct model_point_info
   {
      size_t route_segment_index = 0;                  ///< Номер сегмента маршрута, которому принадлежит точка трека
      MODEL_MOVE_MODE model_mode = MODEL_MOVE_MODE::MMM_FORWARD;   ///< Режим движения модели
   };

   //! Информация о участке маневра
   struct maneuver_route_info
   {
      double dist_to = 0.;                            //< Расстояние до следующего участка маневра, мили
      double direction = 0.;                          //< Направление теущего участка маневра, градусы
      double speed = 0.;                              //< Скорость теущего участка маневра, узлы
      int    time_to = 0;                             //< Время до маневра, секунды
      MODEL_MOVE_MODE maneuver_type;                  //< Тип маневра
      double next_direction = 0.;                     //< Направление следующего участка маневра, градусы
      double next_speed = 0.;                         //< Скорость на следующем участке маневра, узлы
   };

#pragma pack (pop)
} //namespace colreg