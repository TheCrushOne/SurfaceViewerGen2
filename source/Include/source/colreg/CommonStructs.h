#pragma once

#include "BaseRef.h"
#include <limits>

// HACK: переопределяем мин и макс для ухода к стл
#undef min
#undef max

namespace colreg
{
   //! Степень опасности
   enum class DANGER_LEVEL : int
   {
      DL_GREEN = 1, ///< Safe 
      DL_YELLOW,    ///< Caution
      DL_RED        ///< Danger
   };

   enum DOMAIN_TYPE : char
   {
      DOM_RADIUS = 0,  ///< Safety domain is a circle around the ship
      DOM_ELLIPTIC = 1 ///< Safety domain is an elliptical three-sector smooth domain around the ship
   };

#pragma pack (push, 1)

   //using geo_points_ref = base_ref<geo_point>;
   //typedef std::vector<geo_point> geo_points_vct;
   //typedef geo_points_vct& geo_points_vct_ref;

   //template< class POINT_TYPE>
   //struct track_point_info_t
   //{
   //   using POINT_TYPE_CLASS = POINT_TYPE;
   //   POINT_TYPE pos;                  ///< Координаты места
   //   double    course = NO_VALUE;     ///< COG, градусы
   //   double    speed = NO_VALUE;      ///< SOG, узлы
   //   double    STW = NO_VALUE;        ///< Скорость относительно воды, узлы
   //   double    heading = NO_VALUE;    ///< Направление по носу судна, градусы
   //   double    time = INVALID_TIME;   ///< Время определения позиции, секунды
   //   double    left_XTE = NO_VALUE;   ///< Максимальное боковое смещение слева от линии маршрута, мили
   //   double    right_XTE = NO_VALUE;  ///< Максимальное боковое смещение справа от линии маршрута, мили

   //   bool is_valid() const { return pos.is_valid() && speed != colreg::NO_VALUE; }
   //};

   //typedef track_point_info_t<geo_point> track_point_info;

   

   //struct int_pos
   //{
   //   int_pos() = default;
   //   int_pos(const int_pos&)    = default;
   //   int_pos(int_pos&&)noexcept = default;

   //   int_pos(int x, int y)
   //      : x(x)
   //      , y(y)
   //   {}

   //   int x;
   //   int y;
   //};

   //! Коэффициенты сжатия домена безопасности
   /*struct domain_scales
   {
      domain_scales() = default;

      domain_scales(double sidesScale)
      {
         left_scale = right_scale = forward_scale = backward_scale = sidesScale;
      }

      domain_scales(double leftScale, double rightScale, double forwardScale, double backwardScale)
         : left_scale{ leftScale }
         , right_scale{ rightScale }
         , forward_scale{ forwardScale }
         , backward_scale{ backwardScale }
      {}

      bool operator==(const domain_scales& other) const
      {
         return left_scale     == other.left_scale &&   
                right_scale    == other.right_scale &&
                forward_scale  == other.forward_scale &&
                backward_scale == other.backward_scale;
      }

      double left_scale = NO_VALUE;
      double right_scale = NO_VALUE;
      double forward_scale = NO_VALUE;
      double backward_scale = NO_VALUE;
   };*/
#pragma pack (pop)

} //namespace colreg