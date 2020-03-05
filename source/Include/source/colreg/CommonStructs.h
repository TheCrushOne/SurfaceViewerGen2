#pragma once

#include "BaseRef.h"
#include <limits>
namespace colreg
{
   using id_type = unsigned long long int;
   using mask_type = long long int;

   constexpr double NO_VALUE     = std::numeric_limits<double>::max();
   constexpr double INVALID_TIME = std::numeric_limits<double>::max();

   constexpr size_t  MAX_SHIP_CALL_LENGHT = 32;
   constexpr size_t  MAX_SHIP_NAME_LENGHT = 128;
   constexpr size_t  INVALID_INDEX = std::numeric_limits<size_t>::max();
   constexpr id_type INVALID_ID = std::numeric_limits<id_type>::max();

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

   struct geo_point
   {
      geo_point(double lt, double ln)
         : lon(ln)
         , lat(lt)
      {}
      geo_point()                                      = default;
      geo_point(const geo_point&)                      = default;
      geo_point(geo_point&&)noexcept                   = default;
      geo_point& operator= (const geo_point& other)    = default;
      geo_point& operator= (geo_point&& other)noexcept = default;
      
      bool operator==(const geo_point& other) const   { return lon == other.lon && lat == other.lat;  }
      bool operator!=(const geo_point& other) const   { return !(*this == other); }
      bool is_valid() const                           { return lat != NO_VALUE && lon != NO_VALUE; }

      double lon = NO_VALUE;
      double lat = NO_VALUE; ///< Координаты в градусах
   };

   using geo_points_ref = base_ref<geo_point>;

   template< class POINT_TYPE>
   struct track_point_info_t
   {
      using POINT_TYPE_CLASS = POINT_TYPE;
      POINT_TYPE pos;                  ///< Координаты места
      double    course = NO_VALUE;     ///< COG, градусы
      double    speed = NO_VALUE;      ///< SOG, узлы
      double    STW = NO_VALUE;        ///< Скорость относительно воды, узлы
      double    heading = NO_VALUE;    ///< Направление по носу судна, градусы
      double    time = INVALID_TIME;   ///< Время определения позиции, секунды
      double    left_XTE = NO_VALUE;   ///< Максимальное боковое смещение слева от линии маршрута, мили
      double    right_XTE = NO_VALUE;  ///< Максимальное боковое смещение справа от линии маршрута, мили

      bool is_valid() const { return pos.is_valid() && speed != colreg::NO_VALUE; }
   };

   using track_point_info = track_point_info_t<geo_point>;

   struct route_point
   {
      route_point()
         : speed    { NO_VALUE }
         , radius   { NO_VALUE }
         , left_XTE { NO_VALUE }
         , right_XTE{ NO_VALUE }
      {}

      route_point(const colreg::geo_point& p, double v_ = -1, double r = colreg::NO_VALUE, double lEXT = colreg::NO_VALUE, double rEXT = colreg::NO_VALUE)
         : pos(p)
         , speed(v_)
         , radius(r)
         , left_XTE{ lEXT }
         , right_XTE{ rEXT }
      {}

      route_point(const route_point&)                       = default;
      route_point(route_point&&)noexcept                    = default;
      route_point& operator= (const route_point& other)     = default;
      route_point& operator= (route_point&& other)noexcept  = default;

      inline bool operator == (const route_point& b) const { return pos == b.pos; }
      inline operator colreg::geo_point() const            { return pos; }
      inline bool is_valid() const                         { return pos.is_valid(); }

      geo_point pos;
      double    speed;     ///< Скорость, узлы
      double    radius;    ///< Радиус поворота, мили 
      double    left_XTE;  ///< Максимальное боковое смещение слева от линии маршрута, мили
      double    right_XTE; ///< Максимальное боковое смещение справа от линии маршрута, мили
   };

   //! Информация по кораблю
   struct ship_info
   {
      ship_info()
      {
         memset(this, 0, sizeof(ship_info));
         safety_contour = draft = -1;
      }

      char                 name[MAX_SHIP_NAME_LENGHT];         ///< Название судна
      id_type              id;                                 ///< Уникальный идентификатор судна
      int                  length;                             ///< Длина, метры
      int                  width;                              ///< Ширина, метры
      int                  draft;                              ///< Осадка, метры
      int                  deadweight;                         ///< Дедвейт
      bool                 dangerous_cargo;                    ///< Опасный груз
      int                  safety_contour;                     ///< метры, Мин. Глубина для безопасного хождения корабля, может задаваться извне (капитаном). Не может быть меньше draft
   };

   //! Polar coordinates point structure
   struct polar
   {
      double course;   ///< Course to the point, azimuth
      double distance; ///< Distance to the point, miles
   };

   struct int_pos
   {
      int_pos() = default;
      int_pos(const int_pos&)    = default;
      int_pos(int_pos&&)noexcept = default;

      int_pos(int x, int y)
         : x(x)
         , y(y)
      {}

      int x;
      int y;
   };

   //! Коэффициенты сжатия домена безопасности
   struct domain_scales
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
   };
#pragma pack (pop)

} //namespace colreg