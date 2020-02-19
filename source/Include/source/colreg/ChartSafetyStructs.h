#pragma once

#include "CommonStructs.h"
#include "BaseRef.h"

namespace colreg
{
   ///Cтепень опасности при проверке маршрута на карте
    //    enum class MAP_DANGER_LEVEL : int
    //    {
    //       MDT_YELLOW,
    //       MDT_RED,
    //    };

   #define _BIT_(n) (1i64 << n)

   //! Типы опасных объектов карты
   enum OBJECT_TYPE : unsigned long long int
   {
      OT_NONE = 0,

      //! S-57 areas (http://www.s-57.com/)
      OT_DEPTH_AREA               = _BIT_(0),  ///< DEPARE_A
      OT_DREDGED_AREA             = _BIT_(1),  ///< DRGARE_A
      OT_FLOATING_DOCK_AREA       = _BIT_(2),  ///< FLODOC_A
      OT_HULK_AREA                = _BIT_(3),  ///< HULKES_A
      OT_INSHORE_TRAFFIC_ZONE     = _BIT_(4),  ///< ISTZNE_A
      OT_LAND_AREA                = _BIT_(5),  ///< LNDARE_A
      OT_OBSTRUCTION_AREA         = _BIT_(6),  ///< OBSTRN_A
      OT_OFFSHORE_PRODUCTION_AREA = _BIT_(7),  ///< OSPARE_A
      OT_PONTOON_AREA             = _BIT_(8),  ///< PONTON_A
      OT_RECOMMENDED_TRACK_AREA   = _BIT_(9),  ///< RECTRC_A
      OT_RESTRICTED_AREA          = _BIT_(10), ///< RESARE_A
      OT_TRAFFIC_SEPARATION_ZONE  = _BIT_(11), ///< TSEZNE_A
      OT_TSS_LANE_PART            = _BIT_(12), ///< TSSLPT_A
      OT_TWO_WAY_ROUTE_PART       = _BIT_(13), ///< TWRTPT_A
      OT_UNSURVEYED_AREA          = _BIT_(14), ///< UNSARE_A
      OT_WRECK_AREA               = _BIT_(15), ///< WRECKS_A

      //! S-57 lines (http://www.s-57.com/)
      OT_COASTLINE                = _BIT_(16), ///< COALNE_L
      OT_DEPTH_CONTOUR            = _BIT_(17), ///< DEPCNT_L
      OT_FLOATING_DOCK_LINE       = _BIT_(18), ///< FLODOC_L
      OT_LAND_LINE                = _BIT_(19), ///< LNDARE_L
      OT_OBSTRUCTION_LINE         = _BIT_(20), ///< OBSTRN_L
      OT_PONTOON_LINE             = _BIT_(21), ///< PONTON_L
      OT_RECOMMENDED_TRACK_LINE   = _BIT_(22), ///< RECTRC_L
      OT_TRAFFIC_SEPARATION_LINE  = _BIT_(23), ///< TSELNE_L
      OT_TSS_BOUNDARY             = _BIT_(24), ///< TSSBND_L

      //! S-57 points (http://www.s-57.com/)
      OT_HULK_POINT               = _BIT_(25), ///< HULKES_P
      OT_LAND_POINT               = _BIT_(26), ///< LNDARE_P
      OT_OBSTRUCTION_POINT        = _BIT_(27), ///< OBSTRN_P
      OT_SOUNDING                 = _BIT_(28), ///< SOUNDG_P
      OT_UNDERWATER_ROCK          = _BIT_(29), ///< UWTROC_P
      OT_WRECK_POINT              = _BIT_(30), ///< WRECKS_P

      //! Dynamic areas
      OT_DOMAIN_AREA              = _BIT_(31), ///< DOMAIN       - Зона сужения домена безопасности
      OT_XTE_AREA                 = _BIT_(32), ///< XTEAREA       - Зона сужения XTE
      OT_NO_GO_YELLOW_AREA        = _BIT_(33), ///< NOGO         - Зона запрета движения
      OT_NO_GO_RED_AREA           = _BIT_(34), ///< NOGO_YELLOW  - Зона запрета движения

      //! Traffic Statistic Area
      OT_SHIP_DOMAIN_STATISTIC_AREA    = _BIT_(35), ///< зона сбора статистики по расхождению кораблей друг с другом
      OT_CHART_PASSING_STATISTIC_AREA  = _BIT_(36), ///< зона сбора статистики по расхождению кораблей с объектами карты



      OT_AREAS = OT_DEPTH_AREA | OT_DREDGED_AREA | OT_FLOATING_DOCK_AREA | OT_HULK_AREA | OT_INSHORE_TRAFFIC_ZONE | OT_LAND_AREA |
                 OT_OBSTRUCTION_AREA | OT_OFFSHORE_PRODUCTION_AREA | OT_PONTOON_AREA | OT_RECOMMENDED_TRACK_AREA | OT_RESTRICTED_AREA |
                 OT_TRAFFIC_SEPARATION_ZONE | OT_TSS_LANE_PART | OT_TWO_WAY_ROUTE_PART | OT_UNSURVEYED_AREA | OT_WRECK_AREA | 
                  OT_SHIP_DOMAIN_STATISTIC_AREA | OT_CHART_PASSING_STATISTIC_AREA,

      OT_LINES = OT_COASTLINE | OT_DEPTH_CONTOUR | OT_FLOATING_DOCK_LINE | OT_LAND_LINE | OT_OBSTRUCTION_LINE | OT_PONTOON_LINE |
                 OT_RECOMMENDED_TRACK_LINE | OT_TRAFFIC_SEPARATION_LINE | OT_TSS_BOUNDARY,

      OT_POINTS = OT_HULK_POINT | OT_LAND_POINT | OT_OBSTRUCTION_POINT | OT_SOUNDING | OT_UNDERWATER_ROCK | OT_WRECK_POINT,

      OT_DYNAMIC_AREAS = OT_DOMAIN_AREA | OT_NO_GO_YELLOW_AREA | OT_NO_GO_RED_AREA | OT_XTE_AREA,

      // Объекты, глубина которых считается нулевой
      OT_SURFACE_OBJS = OT_FLOATING_DOCK_AREA | OT_HULK_AREA | OT_LAND_AREA | OT_PONTOON_AREA | OT_UNSURVEYED_AREA | 
                        OT_COASTLINE | OT_FLOATING_DOCK_LINE | OT_PONTOON_LINE | OT_HULK_POINT | OT_LAND_POINT | OT_LAND_LINE,

      // Объекты, глубина которых считается не нулевой
      OT_UNDERWATER_OBJS = OT_DEPTH_AREA | OT_DREDGED_AREA | OT_DEPTH_CONTOUR | OT_SOUNDING | OT_UNDERWATER_ROCK | OT_WRECK_POINT | OT_WRECK_AREA,

      OT_TSS = OT_INSHORE_TRAFFIC_ZONE | OT_TRAFFIC_SEPARATION_ZONE | OT_TSS_LANE_PART | OT_TRAFFIC_SEPARATION_LINE | OT_TSS_BOUNDARY | OT_TRAFFIC_SEPARATION_LINE,

      OT_NO_GO_AREAS = OT_NO_GO_YELLOW_AREA | OT_NO_GO_RED_AREA,

      OT_STATISTIC_AREA = OT_SHIP_DOMAIN_STATISTIC_AREA | OT_CHART_PASSING_STATISTIC_AREA,
      OT_ALL = 0xFFFFFFFFFFFFFFFF
   };

   inline bool check_chart_obj_type(colreg::mask_type mask, OBJECT_TYPE type)
   {
      return (mask & type) != 0;
   }

   inline OBJECT_TYPE chart_obj_string_to_type(const char* typeStr)
   {
      // Scenario (остались для поддержки, сохраняться будут под акронимами S-57)
      if (strcmp(typeStr, "COAST") == 0)        return OBJECT_TYPE::OT_LAND_AREA;
      if (strcmp(typeStr, "DEPTH") == 0)        return OBJECT_TYPE::OT_DEPTH_AREA;
      if (strcmp(typeStr, "NOGO") == 0)         return OBJECT_TYPE::OT_NO_GO_RED_AREA;
      if (strcmp(typeStr, "NOGO_YELLOW") == 0)  return OBJECT_TYPE::OT_NO_GO_YELLOW_AREA;
      // Старый формат акронимов S-57 (остались для поддержки, сохраняться будут под новыми)
      if (strcmp(typeStr, "COALNE") == 0)       return OBJECT_TYPE::OT_COASTLINE;
      if (strcmp(typeStr, "DEPARE") == 0)       return OBJECT_TYPE::OT_DEPTH_AREA;
      if (strcmp(typeStr, "DEPCNT") == 0)       return OBJECT_TYPE::OT_DEPTH_CONTOUR;
      if (strcmp(typeStr, "DRGARE") == 0)       return OBJECT_TYPE::OT_DREDGED_AREA;
      if (strcmp(typeStr, "FLODOC") == 0)       return OBJECT_TYPE::OT_FLOATING_DOCK_AREA;
      if (strcmp(typeStr, "HULKES") == 0)       return OBJECT_TYPE::OT_HULK_AREA;
      if (strcmp(typeStr, "ISTZNE") == 0)       return OBJECT_TYPE::OT_INSHORE_TRAFFIC_ZONE;
      if (strcmp(typeStr, "LNDARE") == 0)       return OBJECT_TYPE::OT_LAND_AREA;
      if (strcmp(typeStr, "OBSTRN") == 0)       return OBJECT_TYPE::OT_OBSTRUCTION_AREA;
      if (strcmp(typeStr, "OSPARE") == 0)       return OBJECT_TYPE::OT_OFFSHORE_PRODUCTION_AREA;
      if (strcmp(typeStr, "PONTON") == 0)       return OBJECT_TYPE::OT_PONTOON_AREA;
      if (strcmp(typeStr, "RECTRC") == 0)       return OBJECT_TYPE::OT_RECOMMENDED_TRACK_AREA;
      if (strcmp(typeStr, "RESARE") == 0)       return OBJECT_TYPE::OT_RESTRICTED_AREA;
      if (strcmp(typeStr, "SOUNDG") == 0)       return OBJECT_TYPE::OT_SOUNDING;
      if (strcmp(typeStr, "TSELNE") == 0)       return OBJECT_TYPE::OT_TRAFFIC_SEPARATION_LINE;
      if (strcmp(typeStr, "TSEZNE") == 0)       return OBJECT_TYPE::OT_TRAFFIC_SEPARATION_ZONE;
      if (strcmp(typeStr, "TSSBND") == 0)       return OBJECT_TYPE::OT_TSS_BOUNDARY;
      if (strcmp(typeStr, "TSSLPT") == 0)       return OBJECT_TYPE::OT_TSS_LANE_PART;
      if (strcmp(typeStr, "TWRTPT") == 0)       return OBJECT_TYPE::OT_TWO_WAY_ROUTE_PART;
      if (strcmp(typeStr, "UNSARE") == 0)       return OBJECT_TYPE::OT_UNSURVEYED_AREA;
      if (strcmp(typeStr, "UWTROC") == 0)       return OBJECT_TYPE::OT_UNDERWATER_ROCK;
      if (strcmp(typeStr, "WRECKS") == 0)       return OBJECT_TYPE::OT_WRECK_AREA;
      // S-57
      if (strcmp(typeStr, "DEPARE_A") == 0)     return OBJECT_TYPE::OT_DEPTH_AREA;
      if (strcmp(typeStr, "DRGARE_A") == 0)     return OBJECT_TYPE::OT_DREDGED_AREA;
      if (strcmp(typeStr, "FLODOC_A") == 0)     return OBJECT_TYPE::OT_FLOATING_DOCK_AREA;
      if (strcmp(typeStr, "HULKES_A") == 0)     return OBJECT_TYPE::OT_HULK_AREA;
      if (strcmp(typeStr, "ISTZNE_A") == 0)     return OBJECT_TYPE::OT_INSHORE_TRAFFIC_ZONE;
      if (strcmp(typeStr, "LNDARE_A") == 0)     return OBJECT_TYPE::OT_LAND_AREA;
      if (strcmp(typeStr, "OBSTRN_A") == 0)     return OBJECT_TYPE::OT_OBSTRUCTION_AREA;
      if (strcmp(typeStr, "OSPARE_A") == 0)     return OBJECT_TYPE::OT_OFFSHORE_PRODUCTION_AREA;
      if (strcmp(typeStr, "PONTON_A") == 0)     return OBJECT_TYPE::OT_PONTOON_AREA;
      if (strcmp(typeStr, "RECTRC_A") == 0)     return OBJECT_TYPE::OT_RECOMMENDED_TRACK_AREA;
      if (strcmp(typeStr, "RESARE_A") == 0)     return OBJECT_TYPE::OT_RESTRICTED_AREA;
      if (strcmp(typeStr, "TSEZNE_A") == 0)     return OBJECT_TYPE::OT_TRAFFIC_SEPARATION_ZONE;
      if (strcmp(typeStr, "TSSLPT_A") == 0)     return OBJECT_TYPE::OT_TSS_LANE_PART;
      if (strcmp(typeStr, "TWRTPT_A") == 0)     return OBJECT_TYPE::OT_TWO_WAY_ROUTE_PART;
      if (strcmp(typeStr, "UNSARE_A") == 0)     return OBJECT_TYPE::OT_UNSURVEYED_AREA;
      if (strcmp(typeStr, "WRECKS_A") == 0)     return OBJECT_TYPE::OT_WRECK_AREA;
      if (strcmp(typeStr, "COALNE_L") == 0)     return OBJECT_TYPE::OT_COASTLINE;
      if (strcmp(typeStr, "DEPCNT_L") == 0)     return OBJECT_TYPE::OT_DEPTH_CONTOUR;
      if (strcmp(typeStr, "FLODOC_L") == 0)     return OBJECT_TYPE::OT_FLOATING_DOCK_LINE;
      if (strcmp(typeStr, "LNDARE_L") == 0)     return OBJECT_TYPE::OT_LAND_LINE;
      if (strcmp(typeStr, "OBSTRN_L") == 0)     return OBJECT_TYPE::OT_OBSTRUCTION_LINE;
      if (strcmp(typeStr, "PONTON_L") == 0)     return OBJECT_TYPE::OT_PONTOON_LINE;
      if (strcmp(typeStr, "RECTRC_L") == 0)     return OBJECT_TYPE::OT_RECOMMENDED_TRACK_LINE;
      if (strcmp(typeStr, "TSELNE_L") == 0)     return OBJECT_TYPE::OT_TRAFFIC_SEPARATION_LINE;
      if (strcmp(typeStr, "TSSBND_L") == 0)     return OBJECT_TYPE::OT_TSS_BOUNDARY;
      if (strcmp(typeStr, "HULKES_P") == 0)     return OBJECT_TYPE::OT_HULK_POINT;
      if (strcmp(typeStr, "LNDARE_P") == 0)     return OBJECT_TYPE::OT_LAND_POINT;
      if (strcmp(typeStr, "OBSTRN_P") == 0)     return OBJECT_TYPE::OT_OBSTRUCTION_POINT;
      if (strcmp(typeStr, "SOUNDG_P") == 0)     return OBJECT_TYPE::OT_SOUNDING;
      if (strcmp(typeStr, "UWTROC_P") == 0)     return OBJECT_TYPE::OT_UNDERWATER_ROCK;
      if (strcmp(typeStr, "WRECKS_P") == 0)     return OBJECT_TYPE::OT_WRECK_POINT;
      // Dynamic areas
      if (strcmp(typeStr, "DOMAIN_AREA") == 0)       return OBJECT_TYPE::OT_DOMAIN_AREA;
      if (strcmp(typeStr, "XTE_AREA") == 0)          return OBJECT_TYPE::OT_XTE_AREA;
      if (strcmp(typeStr, "NO_GO_YELLOW_AREA") == 0) return OBJECT_TYPE::OT_NO_GO_YELLOW_AREA;
      if (strcmp(typeStr, "NO_GO_RED_AREA") == 0)    return OBJECT_TYPE::OT_NO_GO_RED_AREA;

      return OBJECT_TYPE::OT_NONE;
   }

   inline const char* chart_obj_type_to_str(OBJECT_TYPE type)
   {
      switch (type)
      {
      // S-57
      case OBJECT_TYPE::OT_COASTLINE:                return "COALNE_L";
      case OBJECT_TYPE::OT_DEPTH_AREA:               return "DEPARE_A";
      case OBJECT_TYPE::OT_DEPTH_CONTOUR:            return "DEPCNT_L";
      case OBJECT_TYPE::OT_DREDGED_AREA:             return "DRGARE_A";
      case OBJECT_TYPE::OT_FLOATING_DOCK_AREA:       return "FLODOC_A";
      case OBJECT_TYPE::OT_FLOATING_DOCK_LINE:       return "FLODOC_L";
      case OBJECT_TYPE::OT_HULK_AREA:                return "HULKES_A";
      case OBJECT_TYPE::OT_HULK_POINT:               return "HULKES_P";
      case OBJECT_TYPE::OT_INSHORE_TRAFFIC_ZONE:     return "ISTZNE_A";
      case OBJECT_TYPE::OT_LAND_AREA:                return "LNDARE_A";
      case OBJECT_TYPE::OT_LAND_LINE:                return "LNDARE_L";
      case OBJECT_TYPE::OT_LAND_POINT:               return "LNDARE_P";
      case OBJECT_TYPE::OT_OBSTRUCTION_AREA:         return "OBSTRN_A";
      case OBJECT_TYPE::OT_OBSTRUCTION_LINE:         return "OBSTRN_L";
      case OBJECT_TYPE::OT_OBSTRUCTION_POINT:        return "OBSTRN_P";
      case OBJECT_TYPE::OT_OFFSHORE_PRODUCTION_AREA: return "OSPARE_A";
      case OBJECT_TYPE::OT_PONTOON_AREA:             return "PONTON_A";
      case OBJECT_TYPE::OT_PONTOON_LINE:             return "PONTON_L";
      case OBJECT_TYPE::OT_RECOMMENDED_TRACK_AREA:   return "RECTRC_A";
      case OBJECT_TYPE::OT_RECOMMENDED_TRACK_LINE:   return "RECTRC_L";
      case OBJECT_TYPE::OT_RESTRICTED_AREA:          return "RESARE_A";
      case OBJECT_TYPE::OT_SOUNDING:                 return "SOUNDG_P";
      case OBJECT_TYPE::OT_TRAFFIC_SEPARATION_LINE:  return "TSELNE_L";
      case OBJECT_TYPE::OT_TRAFFIC_SEPARATION_ZONE:  return "TSEZNE_A";
      case OBJECT_TYPE::OT_TSS_BOUNDARY:             return "TSSBND_L";
      case OBJECT_TYPE::OT_TSS_LANE_PART:            return "TSSLPT_A";
      case OBJECT_TYPE::OT_TWO_WAY_ROUTE_PART:       return "TWRTPT_A";
      case OBJECT_TYPE::OT_UNSURVEYED_AREA:          return "UNSARE_A";
      case OBJECT_TYPE::OT_UNDERWATER_ROCK:          return "UWTROC_P";
      case OBJECT_TYPE::OT_WRECK_AREA:               return "WRECKS_A";
      case OBJECT_TYPE::OT_WRECK_POINT:              return "WRECKS_P";
      // Dynamic areas
      case OBJECT_TYPE::OT_DOMAIN_AREA:              return "DOMAIN_AREA";
      case OBJECT_TYPE::OT_XTE_AREA:                 return "XTE_AREA";
      case OBJECT_TYPE::OT_NO_GO_YELLOW_AREA:        return "NO_GO_YELLOW_AREA";
      case OBJECT_TYPE::OT_NO_GO_RED_AREA:           return "NO_GO_RED_AREA";
      case OBJECT_TYPE::OT_SHIP_DOMAIN_STATISTIC_AREA:        return "SHIP_DOMAIN_STATISTIC_AREA";
      case OBJECT_TYPE::OT_CHART_PASSING_STATISTIC_AREA:           return "CHART_PASSING_STATISTIC_AREA";
      }
      return "UNKNOWN";
   }

   //! Тип объекта карты
   enum class AREA_TYPE : int
   {
      AT_RED_AREA,    ///< Если траектория прогноза проходит через красную зону = опасность.
      AT_YELLOW_AREA, ///< Если траектория прогноза проходит через желтую зону = предупреждение.
   };

   //! Контекст нахождения точки траектории на карте
   enum class POINT_CHART_CONTEXT : char
   {
      PCC_OPEN_WATER = 0,
      PCC_TSS = 1,               ///< На полосе разделения движения
      PCC_NARROW = 2,            ///< В узости
      PCC_NO_GO_YELLOW_AREA = 4, ///< В зоне нежелательного движения
      PCC_XTE_STATISTIC_AREA = 8, ///< В зоне статистики движения
   };

   //! Тип пересечения с контекстной зоной
   enum class CROSS_AREA_CONTEXT : char
   {
      CAC_NO_CROSS = 0,
      CAC_ALONG,        ///< Вдоль направления движения
      CAC_OPPOSITE,     ///< В противоположном направлении относительно основного
      CAC_CROSS         ///< Поперёк направления движения
   };

#pragma pack (push, 1)
   struct geo_point_ex : geo_point
   {
      geo_point_ex(const geo_point& p, double leftXTE, double rightXTE)
         : geo_point(p)
         , left_XTE{ leftXTE }
         , right_XTE{ rightXTE }
      {}

      double left_XTE;  ///< Максимальное боковое смещение слева, мили
      double right_XTE; ///< Максимальное боковое смещение справа, мили
   };

   using geo_points_ex_ref = base_ref<geo_point_ex>;

   //! Описание зоны ограничения движения
   struct no_go_area_info
   {
      AREA_TYPE type;           ///< Тип зоны (AT_RED_AREA - запрет джижения; AT_YELLOW_AREA - движение со штрафом)
      double    penalty = 100.; ///< Штраф за прохождение через зону
   };

   //! Описание зоны сжатия домена безопасности для анализа на карте
   struct domain_area_info
   {
      double left_XTE  = 0.;  ///< Боковое смещение слева для анализа на безопасность по карте, мили
      double right_XTE = 0.;  ///< Боковое смещение справа для анализа на безопасность по карте, мили
    
      domain_scales compression = { 1. };  ///< Коэффициенты сжатия домена

      const char* statistic_info = nullptr; ///< Параметры зоны в виде xml-строчки для вычисления домена
   };

   constexpr unsigned short int STAT_XTE_NO_VALUE = std::numeric_limits<unsigned short int>::max();
   /*!
   \struct chart_context
   Результат анализа траектории на карте
   */
   struct chart_context
   {
      chart_context()
         : context{ (UINT)POINT_CHART_CONTEXT::PCC_OPEN_WATER }
         , cross_context{ CROSS_AREA_CONTEXT::CAC_NO_CROSS }
         , context_width{ NO_VALUE }
         , penalty{ 0. }
         , is_in_danger{ false }
         , id{ INVALID_ID }
         , statistic_left_xte{ STAT_XTE_NO_VALUE }
         , statistic_right_xte{ STAT_XTE_NO_VALUE }
      {}

      UINT                  context;             ///< Контекст нахождения точки на карте как объединение полей из POINT_CHART_CONTEXT
      CROSS_AREA_CONTEXT    cross_context;       ///< Тип пересечения с контекстной зоной
      double                context_width;       ///< Ширина узости в районе точки для context == PCC_NARROW, или ширина полосы TSS для context == PCC_TSS
      double                penalty;             ///< Штраф при движении
      domain_scales         area_scale = { 1. }; ///< Коэффициенты к базовому домену безопасности
      domain_scales         statistic_scale;     ///< Коэффициенты домена безопасности из статистики (к длине корабля)
      bool                  is_in_danger;
      id_type               id;
      unsigned short        statistic_left_xte;  ///< Левый XTE , полученный из статистики, в соответствии с осадкой и курсом корабля, метры. 
      unsigned short        statistic_right_xte; ///< Правый XTE, полученный из статистики, в соответствии с осадкой и курсом корабля, метры. 
   };

   /*!
   \brief ID объектов карты
   Объект может быть либо динамическим, либо статическим
   */
   struct chart_object_id
   {
      id_type static_id = INVALID_ID; ///< ID для статических объектов карты
      id_type dynamic_id = INVALID_ID; ///< ID динамических объектов карты

      bool operator==(const chart_object_id& id) const
      {
         return static_id == id.static_id && dynamic_id == id.dynamic_id;
      }

      inline bool is_valid() const
      {
         if (static_id == INVALID_ID && dynamic_id == INVALID_ID)
            return false;
         return static_id == INVALID_ID || dynamic_id == INVALID_ID; // объект либо статический, либо динамический
      }
   };


   //! Результат проверки маршрута на безопасность
   struct danger_object_info
   {
      OBJECT_TYPE       type;
      double            dist;             ///< Растояние до объекта карты, МИЛИ
      double            distSqr;          ///< Квадрат растояние до объекта карты (в метрах)
      unsigned int      point_index;      ///< Номер точки траектории, где объект представляет опасность
      chart_object_id   danger_id;        ///< ID опасного объекта

      danger_object_info()
         : type(OBJECT_TYPE::OT_NONE)
         , dist(NO_VALUE)
         , distSqr(NO_VALUE)
         , point_index(-1)         
      {}

      bool is_valid() const { return dist != NO_VALUE; }
   };

   //структура хранит минимальные расстояния до опасностей слева и справа для сегмента траектории
   struct segment_min_safe_distances
   {
      double leftDistance = NO_VALUE;
      double rightDistance = NO_VALUE;
   };
   using trajectory_safe_distances_ref = base_ref<segment_min_safe_distances>; //ссылка на массив для одной траектории

   struct safety_route_result
   {
      bool safe = true;
      danger_object_info doi;
      double minSafeDistanceLeft  = NO_VALUE;  //минимальная дистанция до опасности на траектории слева
      double minSafeDistanceRight = NO_VALUE;  //минимальная дистанция до опасности на траектории справа
   };
   

   

   //структура используется в TrafficAnalyzer для анализа картографических опасносях траффика 
   struct safety_route_result_ex
   {
      bool safe = true;
      danger_object_info doi;
      double object_depth_val=colreg::NO_VALUE; // глубина опасного объекта
      geo_point routePoint; //ближайшая точка на сегменте к опасному объекту
      geo_point objectPoint; //ближайший опасный объект
   };

   //! Результат проверки сегмента траектории . Используется в TrafficAnalyzer
   struct segment_check_result
   {
      safety_route_result_ex leftXTECheck;
      safety_route_result_ex rightXTECheck;
   };

   struct simple_prop
   {
      simple_prop() = default;

      simple_prop(const char* k, const char* v)
         : key{ k }
         , val{ v }
      {}

      const char* key;
      const char* val;
   };

   using object_props_ref = base_ref<simple_prop>;

   //! Геометрия объекта карты
   using object_geometry_ref = base_ref<geo_points_ref>;

   
   //! Хэш-функция для использовани ID объектов карты в качестве ключа
   struct chart_object_id_hasher
   {
      std::size_t operator()(const chart_object_id& id) const
      {
         return std::hash<id_type>{}(id.static_id) ^ std::hash<id_type>{}(id.dynamic_id);
      }
   };

   struct chart_object
   {
      chart_object() = default;

      chart_object(OBJECT_TYPE t, const object_geometry_ref& g, const object_props_ref& p)
         : type{ t }, geom{ g }, props{ p }
      {}

      chart_object(chart_object_id i, OBJECT_TYPE t, const object_geometry_ref& g, const object_props_ref& p)
         : id{ i }, type{ t }, geom{ g }, props{ p }
      {}

      chart_object_id id;
      OBJECT_TYPE     type;

      object_geometry_ref geom;
      object_props_ref    props; ///< Свойства объекта
   };

   using chart_objects_ref = base_ref<chart_object>;

   const char PROPS_DELIMITER = '|';

   /*!
   \brief Свойства объектов карты (не совпадает с реальными св-вами объектов S-57)
   Несколько ключей разделяются c помощью PROPS_DELIMITER
   */
   inline const char* chart_obj_props(OBJECT_TYPE type)
   {
      if (check_chart_obj_type(OBJECT_TYPE::OT_UNDERWATER_OBJS, type))
         return "DEPTH";
      if (check_chart_obj_type(OBJECT_TYPE::OT_TSS, type))
         return "ORIENT";
      if (check_chart_obj_type(OBJECT_TYPE::OT_NO_GO_AREAS, type))
         return "PENALTY";
      if (check_chart_obj_type(OBJECT_TYPE::OT_DOMAIN_AREA, type))
         return "LEFT_COMPRESSION|RIGHT_COMPRESSION|FORWARD_COMPRESSION|BACKWARD_COMPRESSION|OFFSET_LEFT_XTE|OFFSET_RIGHT_XTE|STATISTIC_INFO";
      if (check_chart_obj_type(OBJECT_TYPE::OT_XTE_AREA, type))
         return "COG_FROM|COG_TO|DRAFT_FROM|DRAFT_TO|ZONE_TYPE|START_POINT_LAT|START_POINT_LON|END_POINT_LAT|END_POINT_LON|TRAFFIC_WIDTH_LEFT|TRAFFIC_WIDTH_RIGHT|OFFSET_LEFT_XTE|OFFSET_RIGHT_XTE|STATISTIC_INFO";
      return nullptr;
   }

   //! Получить строковое значение по-умолчанию для свойства объекта карты
   inline const char* chart_obj_prop_default(const char* prop)
   {
      if (strcmp(prop, "DEPTH") == 0 ||
          strcmp(prop, "ORIENT") == 0 ||
          strcmp(prop, "OFFSET_LEFT_XTE") == 0 ||
          strcmp(prop, "OFFSET_RIGHT_XTE") == 0 )
         return "0.";
      if (strcmp(prop, "PENALTY") == 0)
         return "100.";
      if (strcmp(prop, "LEFT_COMPRESSION")     == 0 ||
          strcmp(prop, "RIGHT_COMPRESSION")    == 0 ||
          strcmp(prop, "FORWARD_COMPRESSION")  == 0 ||
          strcmp(prop, "BACKWARD_COMPRESSION") == 0)
         return "1.";
      if (strcmp(prop, "STATISTIC_INFO") == 0)
         return "";

      if (strcmp(prop, "DRAFT_FROM") == 0 || strcmp(prop, "DRAFT_TO") == 0)
      {
         return "3";
      }

      if (strcmp(prop, "COG_FROM") == 0 || strcmp(prop, "COG_TO") == 0)
      {
         return "0";
      }

      if (strcmp(prop, "ZONE_TYPE") == 0 )
      {
         return "1";
      }

      if (strcmp(prop, "START_POINT_LAT") == 0 || strcmp(prop, "START_POINT_LON") == 0 ||
         strcmp(prop, "END_POINT_LAT") == 0 || strcmp(prop, "END_POINT_LON") == 0 )
      {
         return "0.0";
      }

      if (strcmp(prop, "TRAFFIC_WIDTH_LEFT") == 0 || strcmp(prop, "TRAFFIC_WIDTH_RIGHT") == 0)
      {
         return "200.0";
      }

      return nullptr;
   }

#pragma pack (pop)
} //namespace colreg