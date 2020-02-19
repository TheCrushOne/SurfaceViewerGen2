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

   //! Приоритет судов
   enum class SHIP_PRIORITY : int
   {
      SPR_SELF = 0,            ///< Приоритет своего судна 
      SPR_PARITET,             ///< Нет приоритетов у своего судна и у цели
      SPR_TARGET,              ///< Приоритет судна цели
      SPR_SELF_CAN_GIVE_WAY,   ///< Приоритет своего цели, но можем уступить дорогу (в узком проходе или фарватере, если обгон может быть совершен только когда обгоняемое судно предпримет действие - Правило 9е)
      SPR_TARGET_CAN_GIVE_WAY, ///< Приоритет судна цели, но может уступить дорогу (в узком проходе или фарватере, если обгон может быть совершен только когда обгоняемое судно предпримет действие - Правило 9е)
   };

   //! Степень опасности
   enum class DANGER_LEVEL : int
   {
      DL_GREEN = 1, ///< Safe 
      DL_YELLOW,    ///< Caution
      DL_RED        ///< Danger
   };

   //! Тип маневра
   enum class MANEUVER_TYPE : int
   {
      MT_TURN_R = 0,           ///< Изменение курса вправо
      MT_TURN_L,               ///< Изменение курса влево
      MT_SPEED_UP,             ///< Увеличение скорости
      MT_SPEED_DOWN,           ///< Уменьшение скорости
      MT_EMERGENCY_SPEED_DOWN, ///< Экстренный маневр уменьшением скорости
      MT_TURN_35R,             ///< Поворот вправо на обратный курс
      MT_TURN_35L,             ///< Поворот влево на обратный курс
      MT_CIR_R,                ///< Циркуляция 360 градусов вправо 
      MT_CIR_L,                ///< Циркуляция 360 градусов влево
      MT_SPEED_UP_AND_TURN_R,  ///< Набрать скорость и повернуть вправо
      MT_SPEED_UP_AND_TURN_L,  ///< Набрать скорость и повернуть влево
      MT_KEEP_WAY_DOMAIN_LOOSE,///< Движение по текущему маршруту с уменьшением домена
   };

   inline const char* maneuver_to_string(MANEUVER_TYPE t)
   {
      switch (t)
      {
      case MANEUVER_TYPE::MT_TURN_R:               return "Right turn";
      case MANEUVER_TYPE::MT_TURN_L:               return "Left turn";
      case MANEUVER_TYPE::MT_SPEED_UP:             return "Increase speed";
      case MANEUVER_TYPE::MT_SPEED_DOWN:           return "Decrease speed";
      case MANEUVER_TYPE::MT_EMERGENCY_SPEED_DOWN: return "Emergency stop";
      case MANEUVER_TYPE::MT_TURN_35R:             return "Right Uturn";
      case MANEUVER_TYPE::MT_TURN_35L:             return "Left Uturn";
      case MANEUVER_TYPE::MT_CIR_R:                return "Right circulation";
      case MANEUVER_TYPE::MT_CIR_L:                return "Left circulation";
      case MANEUVER_TYPE::MT_SPEED_UP_AND_TURN_R:  return "Increase speed and turn right";
      case MANEUVER_TYPE::MT_SPEED_UP_AND_TURN_L:  return "Increase speed and turn left";
      case MANEUVER_TYPE::MT_KEEP_WAY_DOMAIN_LOOSE:return "Keep way with reduced domain";
      }
      return "Unknown maneuver";
   }


   //! Тип завершения маневра
   enum class MANEUVER_RETUTN_TYPE : int
   {
      MRT_PREDICTION = 0,      ///< Возвращаемся на траекторию прогноза/маршрута
      MRT_PREDICTION_DIRECTION,///< Отворачиваем и возвращаемся на направление прогноза/маршрута
      MRT_TURN_ASIDE,          ///< Отворачиваем в сторону
   };

   //! Тип маршрута
   enum class ROUTE_TYPE : int
   {
      RT_MAIN = 0,             ///< Главный маршрут
      RT_MANEUVER,             ///< Маневр для избежания опасной ситуации, полученной в GetEstimation
   };

   //! Тип первого маневра
   enum class FIRST_MANEUVER_TYPE : int
   {
      FMT_NONE = 0,            ///< Не задан
      FMT_TURN_LEFT,           ///< Налево
      FMT_TURN_RIGHT,          ///< Направо
      FMT_SPEED_UP,            ///< Увеличение скорости
      FMT_SPEED_DOWN,          ///< Уменьшение скорости
      FMT_CIR_LEFT,            ///< Циркуляция 360 градусов влево
      FMT_CIR_RIGHT,           ///< Циркуляция 360 градусов вправо 
      FMT_KEEP_COURSE_AND_SPEED///< Сохранять курс и скорость
   };

   //! Тип форсированного маневра
   enum class FORCE_MANEUVER_TYPE : int
   {
      FMT_NONE = 0,  
      FMT_FORCE_GIVE_WAY,       ///< Принудительно уступить дорогу 
      FMT_FORCE_WAIT_LAST_MOMENT///< Принудительно искать маневр последнего момента
   };
   
   inline FIRST_MANEUVER_TYPE toFirstManeuver(MANEUVER_TYPE type)
   {
      switch (type)
      {
      case MANEUVER_TYPE::MT_TURN_R:
      case MANEUVER_TYPE::MT_SPEED_UP_AND_TURN_R:
         return FIRST_MANEUVER_TYPE::FMT_TURN_RIGHT;
      
      case MANEUVER_TYPE::MT_TURN_L:
      case MANEUVER_TYPE::MT_SPEED_UP_AND_TURN_L:
         return FIRST_MANEUVER_TYPE::FMT_TURN_LEFT;
      
      case MANEUVER_TYPE::MT_SPEED_UP:
         return FIRST_MANEUVER_TYPE::FMT_SPEED_UP;
      
      case MANEUVER_TYPE::MT_SPEED_DOWN:
      case MANEUVER_TYPE::MT_EMERGENCY_SPEED_DOWN:
         return FIRST_MANEUVER_TYPE::FMT_SPEED_DOWN;
      
      case MANEUVER_TYPE::MT_TURN_35R:
      case MANEUVER_TYPE::MT_CIR_R:
         return FIRST_MANEUVER_TYPE::FMT_CIR_RIGHT;
      
      case MANEUVER_TYPE::MT_TURN_35L:
      case MANEUVER_TYPE::MT_CIR_L:
         return FIRST_MANEUVER_TYPE::FMT_CIR_LEFT;

      case MANEUVER_TYPE::MT_KEEP_WAY_DOMAIN_LOOSE:
         return FIRST_MANEUVER_TYPE::FMT_KEEP_COURSE_AND_SPEED;
      }
      ATLASSERT(!"Unsupported maneuver type");
      return FIRST_MANEUVER_TYPE::FMT_NONE;
   }

   inline bool isSpeedManeuver(MANEUVER_TYPE action)
   {
      return action == MANEUVER_TYPE::MT_SPEED_UP || action == MANEUVER_TYPE::MT_SPEED_DOWN || action == MANEUVER_TYPE::MT_EMERGENCY_SPEED_DOWN;
   }

   inline bool isTurnBackManeuver(MANEUVER_TYPE action)
   {
      return action == MANEUVER_TYPE::MT_TURN_35R || action == MANEUVER_TYPE::MT_TURN_35L || action == MANEUVER_TYPE::MT_CIR_R || action == MANEUVER_TYPE::MT_CIR_L;
   }

   //! Правила МППСС: http://www.mar.ist.utl.pt/mventura/Projecto-Navios-I/IMO-Conventions%20(copies)/COLREG-1972.pdf
   enum class COLREG_RULE : int
   {
      CR_UNDEFINED = 0,
      // общие положения
      //       CR_APPLICATION = 1,
      //       CR_RESPONSIBILITY = 2,
      //       CR_GENERAL_DEFINITIONS = 3,
      //       // Раздел I - Плавание судов при любых условиях видимости
      //       CR_APPLICATION_VIS = 4,
      //       CR_LOOK_OUT = 5,
      //       CR_SAFE_SPEED = 6,
      //       CR_RISK_OF_COLLISION = 7,
      //       CR_ACTION_TO_AVOID_COLLISION = 8,
      CR_NARROW_CHANNELS = 9,
      CR_TSS = 10,
      //       // Раздел II - Плавание судов, находящихся на виду друг у друга
      //       CR_APPLICATION_VESSELS_IN_SIGHT = 11,
      //       CR_SAILING_VESSELS = 12,
      CR_OVERTAKING = 13,
      CR_HEAD_ON_SITUATION = 14,
      CR_CROSSING_SITUATION = 15,
      CR_ACTION_BY_GIVE_WAY_VESSEL = 16,
      CR_ACTION_BY_STAND_ON_VESSEL = 17,
      //       CR_RESPONSIBILITIES_BETWEEN_VESSELS = 18,
      //       //Раздел III - Плавание судов при ограниченной видимости
      //       CR_CONDUCT_OF_VESSELS_IN_RESTRICTED_VISIBILITY = 19,
      //       // Часть C - Огни и знаки
      //       CR_APPLICATION_LIGHTS = 20,
      //       CR_DEFINITIONS_LIGHTS = 21,
      //       CR_VISIBILITY_OF_LIGHTS = 22,
      //       CR_POWER_DRIVEN_VESSELS_UNDERWAY = 23,
      //       CR_TOWING_AND_PUSHING = 24,
      //       CR_SAILING_VESSELS_UNDERWAY_AND_VESSELS_UNDER_OARS = 25,
      //       CR_FISHING_VESSELS = 26,
      //       CR_RESTRICTED_VESSELS = 27,
      //       CR_VESSELS_CONSTRAINED_BY_THEIR_DRAUGHT = 28,
      //       CR_PILOT_VESSELS = 29,
      //       CR_ANCHORED_VESSELS_AND_VESSELS_AGROUND = 29,
      //       CR_SEAPLANES = 30,
      //       // Часть D - Звуковые и световые сигналы
      //       CR_APPLICATION_SIGNALS = 31,
      //       CR_DEFINITIONS_SIGNALS = 32,
      //       CR_EQUIPMENT_FOR_SOUND_SIGNALS = 33,
      //       CR_MANOEUVRING_AND_WARNING_SIGNALS = 34,
      //       CR_SOUND_SIGNALS_IN_RESTRICTED_VISIBILITY = 35,
      //       CR_SIGNALS_TO_ATTRACT_ATTENTION = 36,
      //       CR_DISTRESS_SIGNALS = 37,
      //       CR_EXEMPTIONS = 38
   };

   enum class SHIP_RELATION : int
   {
      SR_NONE = 0,
      SR_HEAD_ON,                            ///< A, Ситуация сближения друг на друга
      SR_CROSSING_RIGHT,                     ///< B, Ситуация пересечения курсов справа
      SR_CROSSING_LEFT,                      ///< C, Ситуация пересечения курсов слева
      SR_OVERTAKING_TARGET,                  ///< D, Ситуация обгон цели
      SR_OVERTAKING_OWN,                     ///< E, Ситуация обгон нашего судна
      SR_PARALLEL_OVERTAKING_TARGET,         ///< ?, Ситуация параллельного обгона цели
      SR_PARALLEL_OVERTAKING_OWN,            ///< ?, Ситуация параллельного обгона нашего судна
      SR_GIVE_WAY,                           ///< F, Ситуация уход с курса цели
      SR_PARALLEL,                           ///< G, Ситуация параллельным курсом 
      SR_PARALLEL_HEAD_ON,                   ///< H, Ситуация параллельным встречным курсом
   };

   //! Тип решения
   enum class SOLUTION_TYPE : int
   {
      ST_PASSING = 1,                        ///< Safe 
      ST_LAST_MOMENT,                        ///< Маневры последнего момента
      ST_KEEP_WAY,                           ///< Траектория не изменилась
      ST_NOT_FOUND,                          ///< Безопасные маневры не найдены
      ST_SELECTING,                          ///< Маневры уже найдены и доступны через iStateFullNavEstimation::GetShipById(id)->GetPosibleManeuvers()
      ST_DANGER_SELECTING,                   ///< Маневры уже найдены для опасного корабля и доступны через iStateFullNavEstimation::GetShipById(dangerId)->GetPosibleManeuvers()
   };

   //! Послабления, применяемые при поиске решения
   enum class SETTINGS_CORRECTION_TYPE : int
   {
      SCT_NONE = 0,                          ///< Без послаблений
      SCT_REDUCE_CLUSTER_SHIPS_START_TIME,   ///< Уменьшено время старта для опасных кораблей кластера
      SCT_REDUCE_DANGER_SHIPS_DOMAIN,        ///< Уменьшены домены для текущего и опасного кораблей
      SCT_LOOSENING_TYPES_COUNT
   };

   //! Тип препятствия
   enum class OBSTACLE_TYPE : int
   {
      OT_NONE = 0,
      OT_CHART_OBJECT,                       ///< Картографический объект
      OT_VESSEL,                             ///< Судно
      OT_WEATHER_CONDITION,                  ///< Погодные условия
   };

   //! Тип предупреждения
   enum class WARNING_TYPE
   {
      WT_NOT_ON_ROUTE = 0,                   ///< Судно движется не по проложенному маршруту
      WT_CROSS_TSS,                          ///< Судно движется по пересекающeйся с TSS траектории 
      WT_CROSS_NOGO_YELLOW_AREA,             ///< Судно движется в зону нежелательную для движения
      WT_OPPOSITE_MOVE,                      ///< Судно движется по "встречке"
      WT_POSIBLE_DANGER,                     ///< Есть неподтверженная вероятность возникновения опасности 
                                             ///< (probability = вероятность возникновения опасности, а не столкновения)
      
      WT_OUT_OF_ROUTE_BAD_COURSE,            ///< У корабля курс сильно отличается маршрутного
      WT_OUT_OF_ROUTE_XTE_EXCEEDED,          ///< Корабль вышел за границы маршрута
      WT_DOMAIN_REDUCED,                     ///< Корабль плывет с временно уменьшенным доменом
      WT_WITHIN_CHART_DANGER,                ///< Корабль находится внутри опасного объекта карты (например, DEEP AREA)

      WT_TTL_ERROR,                          ///< Корабль исключен из анализа, т.к. его позиция слишком давно не обновлялась. 

      //weather
      WT_WEATHER_CONDITION,                   ///< Корабль не может пройти по маршруту из-за погодных условий

   };

   enum class NAV_STATUS : int
   {
      NAV_UNDERWAYUSINGENGINE = 0,           ///< Cудно с механическим двигателем на ходу
      NAV_ATANCHOR,                          ///< Судно на якоре
      NAV_NOTUNDERCOMMAND,                   ///< Судно лишенное возможности управления
      NAV_RESTRICTDMANEUVERABILITY,          ///< Судно ограниченное в маневрирование
      NAV_CONSTRAINEDBYHERDRAUGHT,           ///< Судно стесненное своей осадкой
      NAV_MOORED,                            ///< Судно пришвартовано
      NAV_AGROUND,                           ///< Судно на мели
      NAV_ENGAGEDINFISHING,                  ///< Судно, занятое ловом рыбы, на ходу 
      NAV_UNDERWAYSAILING,                   ///< Судно под парусом на ходу
      NAV_NOTDEFINEDDEFAULT                  ///< Статус судна не установлен = по умолчанию (также используется АИС-SART тестируемого)
   };
   
   enum class USER_NAV_STATUS : int
   {
      UNS_NONE = 0,
      UNS_NOT_ACCEPT_ROUTE, ///< Cудно не принимает маршрут      
   };

   //! Тип судна
   enum class VESSEL_STATUS : int
   {
      MOD_UNKNOWN = 0,                       ///< "NO information"
      MOD_GROUND,                            ///< "Wing in ground"
      MOD_FISHING,                           ///< "Fishing"
      MOD_TOWING,                            ///< "Towing"
      MOD_LARGETOWING,                       ///< "Towing: length exceeds 200m or breadth exceeds 25m"
      MOD_UNDERWATER,                        ///< "Dredging or underwater ops"
      MOD_DIVING,                            ///< "Diving ops"
      MOD_MILITARY,                          ///< "Military ops"
      MOD_SAILING,                           ///< "Sailing"
      MOD_PLEASURE,                          ///< "Pleasure Craft"
      MOD_HISPEED,                           ///< "High speed craft"
      MOD_PILOT,                             ///< "Pilot Vessel"
      MOD_SARSHIP,                           ///< "Search and Rescue vessel"
      MOD_TUG,                               ///< "Tug"
      MOD_TENDER,                            ///< "Port Tender"
      MOD_ANTIPOLUTION,                      ///< "Anti-pollution equipment"
      MOD_LOWENFORCE,                        ///< "Law Enforcement"
      MOD_LOCALVESSEL,                       ///< "Local Vessel"
      MOD_MEDICAL,                           ///< "Medical Transport"
      MOD_NONCOMBATANT,                      ///< "Noncombatant ship"
      MOD_PASSENGER,                         ///< "Passenger"
      MOD_CARGO,                             ///< "Cargo"
      MOD_TANKER,                            ///< "Tanker"
      MOD_OTHER                              ///< "Other Type" 
   };
   

   inline const char* vessel_status_to_string(VESSEL_STATUS s)
   {
      switch (s)
      {
      case VESSEL_STATUS::MOD_UNKNOWN: return "NO information";
      case VESSEL_STATUS::MOD_GROUND: return "Wing in ground";
      case VESSEL_STATUS::MOD_FISHING: return "Fishing";
      case VESSEL_STATUS::MOD_TOWING: return "Towing";
      case VESSEL_STATUS::MOD_LARGETOWING: return "Towing: length exceeds 200m or breadth exceeds 25m";
      case VESSEL_STATUS::MOD_UNDERWATER: return "Dredging or underwater ops";
      case VESSEL_STATUS::MOD_DIVING: return "Diving ops";
      case VESSEL_STATUS::MOD_MILITARY: return "Military ops";
      case VESSEL_STATUS::MOD_SAILING: return "Sailing";
      case VESSEL_STATUS::MOD_PLEASURE: return "Pleasure Craft";
      case VESSEL_STATUS::MOD_HISPEED: return "High speed craft";
      case VESSEL_STATUS::MOD_PILOT: return "Pilot Vessel";
      case VESSEL_STATUS::MOD_SARSHIP: return "Search and Rescue vessel";
      case VESSEL_STATUS::MOD_TUG: return "Tug";
      case VESSEL_STATUS::MOD_TENDER: return "Port Tender";
      case VESSEL_STATUS::MOD_ANTIPOLUTION: return "Anti-pollution equipment";
      case VESSEL_STATUS::MOD_LOWENFORCE: return "Law Enforcement";
      case VESSEL_STATUS::MOD_LOCALVESSEL: return "Local Vessel";
      case VESSEL_STATUS::MOD_MEDICAL: return "Medical Transport";
      case VESSEL_STATUS::MOD_NONCOMBATANT: return "Noncombatant ship";
      case VESSEL_STATUS::MOD_PASSENGER: return "Passenger";
      case VESSEL_STATUS::MOD_CARGO: return "Cargo";
      case VESSEL_STATUS::MOD_TANKER: return "Tanker";
      case VESSEL_STATUS::MOD_OTHER: return "Other Type";
      }
      return "";
   };

   //! Дополнительные условия при обработке на наличие опасностей
   enum class DANGER_OVERRIDE_TYPE : int
   {
      DOT_DEFAULT = 0, ///< Нет дополнительных условий
      DOT_EXCLUDED,    ///< Судно всегда безопасно
      DOT_STATUS       ///< Судно безопасно, если это буксир (MOD_TUG) или (MOD_TENDER)
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
         safety_contour = draft = telegraph= -1;
      }

      char                 name[MAX_SHIP_NAME_LENGHT];         ///< Название судна
      char                 call_sign[MAX_SHIP_CALL_LENGHT];    ///< Позывной 
      id_type              id;                                 ///< Уникальный идентификатор судна
      unsigned int         MMSI;                               ///< MMSI номер
      unsigned int         IMO;                                ///< IMO номер
      int                  length;                             ///< Длина, метры
      int                  width;                              ///< Ширина, метры
      int                  draft;                              ///< Осадка, метры
      int                  deadweight;                         ///< Дедвейт
      NAV_STATUS           AIS_status;                         ///< Статус по АИС
      USER_NAV_STATUS      user_status;                        ///< Статус, установленный оператором
      VESSEL_STATUS        type;                               ///< Тип судна    
      double               grounding_safe_left;                ///< Безопасное расстояние до объектов карты слева, метры
      double               grounding_safe_right;               ///< Безопасное расстояние до объектов карты справа, метры
      DANGER_OVERRIDE_TYPE danger_override;                    ///< Дополнительные условия опасности судна
      bool                 dangerous_cargo;                    ///< Опасный груз
      int                  safety_contour;                     ///< метры, Мин. Глубина для безопасного хождения корабля, может задаваться извне (капитаном). Не может быть меньше draft
      int                  telegraph;                          ///< telegraph: 0-100, [%]
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

   //! Информация по опасному сближению двух кораблей
   struct danger_approach_record
   {
      long long recordId;
      //! Информация о скорости, направлении и координатах корабля, чей домен был нарушен другим кораблем
      track_point_info  baseShipTrackInfo;
      //! ID корабля, чей домен был нарушен другим кораблем
      id_type           baseShipId;

      //! Информация о скорости, направлении и координатах корабля-нарушителя домена
      track_point_info  intruderShipTrackInfo;
      //! ID корабля-нарушителя домена
      id_type           intruderShipId;


      // точка сближения  
      geo_point         approachPt;

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

   enum class MANEUVER_QUALITY : int
   {
      MQ_NONE = 0,
      MQ_POOR ,
      MQ_AVERAGE,
      MQ_GOOD,
      CONTARY_COLREG 
   };


   // ! Режим работы COLREG
   enum class COLREG_MODE : int
   {
      CM_STATE_LESS,    //< Без хранения предыдущих состояний
      CM_STATE_FULL     //< С хранением предыдущих состояний
   };


   //Стратегии корректировки скорости маршрута
   enum class ROUTE_SPEED_UPDATE_STRATEGY : char
   {
      RSUS_SIMPLE = 0,     ///< Корректируем только первую предстоящую точку маршрута
      RSUS_PROGRESSIVE,   ///< Корректирум первую предстоящую точку, и все последующие с такой же скоростью как и у первой
      RSUS_PROGRESSIVE_GE,   ///< Корректирум первую предстоящую точку, и все последующие у которых скорость больше или равна текущей
      RSUS_FORCE_EQUAL,             ///< всем последующим точкам маршрута выставляется одинаковая скорость

   };
#pragma pack (pop)

} //namespace colreg