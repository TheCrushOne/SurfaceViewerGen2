#pragma once
#include "CommonStructs.h"
#include "CommonRefs.h"
#include "ColregSettings.h"
#include "ModelInterface.h"
#include "DomainInterface.h"
#include "DebugStructs.h"

struct ICommunicator;

namespace colreg
{
#pragma pack (push, 1)

   struct iChartSafetyCheck;
   struct iMathModelManager;
   struct iWeatherInfluence;
   namespace statefull
   {
      struct iStateFullExtention;
      struct iStateFullNavEstimation;
   }

   //! Информация о ситуации встречи
   struct relation_situation
   {
      SHIP_RELATION type = SHIP_RELATION::SR_NONE;    //< Тип отношения (ситуации)
      COLREG_RULE rule1 = COLREG_RULE::CR_UNDEFINED;
      COLREG_RULE rule2 = COLREG_RULE::CR_UNDEFINED;
   };
   
   struct ship_relation_info
   {
      id_type       ship_id;                          //< Идентификатор корабля
      SHIP_PRIORITY priority_AIS;                     //< Приоритет по статусу АИС 
      SHIP_PRIORITY priority_COLREG;                  //< Приоритет текущей ситуации встречи по правилу МППСС
      SHIP_PRIORITY priority_complex;                 //< Комплексный приоритет (prAIS выше, чем prMPPSS за исключением случаев, регламентируемых Правилами 9,10,13.)
      DANGER_LEVEL  danger_level;                     //< Безопасность
      relation_situation situation;                   //< Ситуация встречи
      char           probability;                     //< Вероятность опасного сближения (вхождения в домен безопасности)
   };
   
   using relations_info_ref = base_ref<ship_relation_info>;

   //! Информация об опасном корабле
   struct danger_ship_info
   {
      id_type              ship_id;                   //< ID корабля, для которого возникла опасность
      id_type              danger_id;                 //< ID препятствия
      OBSTACLE_TYPE        obstacle_type;             //< Тип препятствия
      OBJECT_TYPE          chart_object_type;         //< Тип опасного объекта карты
      double               danger_time;               //< Время через которое возникнет опасность, в сек
      double               danger_dist;               //< Опасная дистанция на которой расходимся, мили
      double               dist_to_danger;            //< дистанция до опасности, мили
      track_point_info     posDanger;                 //< позиция ship_id на момент опасности
      track_point_info     posDangerTarget;           //< позиция danger_id на момент опасности
      relations_info_ref   relations;                 //< Отношения с кораблями
   };

   //! Описание маневра расхождения для тестирования
   struct test_description
   {
      double penalty = 0.;
      double time_start = 0.;
      double time_maneuver = 0.;
   };

   //! Описание маневра расхождения
   struct maneuver_description
   {
      test_description test_description;                                       //< Информация для unit-тестов
      MANEUVER_TYPE action             = MANEUVER_TYPE(-1);                    //< Тип маневра
      MANEUVER_RETUTN_TYPE returnType  = MANEUVER_RETUTN_TYPE::MRT_PREDICTION; //< Тип завершения маневра
      size_t start_idx                 = INVALID_INDEX;                        //< Индекс точки начала маневра расхождения
      size_t size                      = 0;                                    //< Количество точек в маневре расхождения
      double overheadTime              = INVALID_TIME;                         //< Накладные расходы по времени на выполнение маневра, секунды
      double overheadDist              = NO_VALUE;                             //< Накладные расходы по дистанции на выполнение маневра, мили
      MANEUVER_QUALITY  quality        = MANEUVER_QUALITY::MQ_NONE;            //< Качество маневра
   };

   //! Информация о послаблениях, используемых при поиске решения
   struct settings_correction
   {
      settings_correction() = default;

      settings_correction(SETTINGS_CORRECTION_TYPE t)
         : type{ t }
      {}

      settings_correction(SETTINGS_CORRECTION_TYPE t, double startTime, const domain_scales& domainScales)
         : type{ t }, start_time{ startTime }, domain_scale{ domainScales }
      {}

      SETTINGS_CORRECTION_TYPE type = SETTINGS_CORRECTION_TYPE::SCT_NONE;

      double start_time = INVALID_TIME;      ///< Коэффициент сжатия времени старта (type == SCT_REDUCE_CLUSTER_SHIPS_START_TIME)
      domain_scales domain_scale;            ///< Коэффициенты сжатия домена (type == SCT_REDUCE_DANGER_SHIPS_DOMAIN)
   };

   //! Описание решения
   struct solution_description
   {
      solution_description() = default;

      solution_description(id_type id, SOLUTION_TYPE t, OBSTACLE_TYPE o, const settings_correction& l, id_type dngr_id, FIRST_MANEUVER_TYPE fm)
         : ship_id{ id }
         , type{ t }
         , obstacle_type{ o }
         , loosening{ l }
         , danger_id{ dngr_id  }
         , firstManeuver{ fm }
      {}

      id_type              ship_id;                                        ///< ID корабля, для которого получено решение
      SOLUTION_TYPE        type;                                           ///< Тип решения
      OBSTACLE_TYPE        obstacle_type;                                  ///< Тип препятствия
      id_type              danger_id;                                      ///< ID препятствия
      settings_correction  loosening;                                      ///< Послабления, применяемые при поиске решения
      FIRST_MANEUVER_TYPE  firstManeuver = FIRST_MANEUVER_TYPE::FMT_NONE;  ///< Тип маневра, заданный пользователем
   };
  
   //! Информация о предупреждении
   struct ship_warning
   {
      id_type      ship_id;                  ///< ID корабля
      double       warning_time;             ///< Время через которое возникнет предупреждение, в сек
      WARNING_TYPE type;                     ///< Тип
      char         probability = 0;          ///< Вероятность наступления события-предупреждения
   };

   struct colreg_paths
   {
      const char* data_path = nullptr;       ///< Путь к папке ColregData (если не задан, то рядом с бинарником)
      const char* log_path = nullptr;        ///< Путь к папке логов (если не задан - то ColregData\CallLogs)
   };

   using warnings_ref            = base_ref<ship_warning>;
   using ids_ref                 = base_ref<id_type>;
   using maneuver_route_info_ref = base_ref<maneuver_route_info>;
   using danger_ship_info_ref    = base_ref<danger_ship_info>;
   using model_name_ref          = base_ref<const char*>;

   //! Вариант решения
   struct iSolutionManeuver
   {
      /*!
      Получить маршрут маневра расхождения корабля, встроенный в его исходный маршрут.
      \return nullptr если маршрута нет
      */
      virtual const route_ref* GetRoute() const = 0;
      
      //! Получить описание маневра расхождения
      virtual maneuver_description GetManeuverDescription() const = 0;
      
      /*!
      Получить описание маневра расхождения в виде последовательности действий
      \return nullptr если маршрута расхождения нет
      */
      virtual const maneuver_route_info_ref* GetManeuverInfo() const = 0;

      /*!
      Получить только маршрут маневра расхождения
      \return nullptr если маршрута нет
      */
      virtual const route_ref* GetManeuverRoute() const = 0;     

   };

   //! Кооперативное решение (один вариант)
   struct iCooperativeManeuver : iSolutionManeuver
   {
      virtual solution_description GetDescription() const = 0;
   };

   struct iCooperativeSolution
   {
      //! Получить id кораблей участвующих в кооперативном решении
      virtual const ids_ref* GetShipIds() const = 0;

      //! Получить манёвр корабля в кооперативном решении
      virtual const iCooperativeManeuver* GetManeuver(id_type shipId) const = 0;
   };

   using solution_maneuver_ref = base_ref<iSolutionManeuver*>;

   //! Одиночное решение
   struct iSingleSolution
   {
      //! Получить описание решения
      virtual solution_description GetDescription() const = 0;

      //! Получить варианты безопасного расхождения
      virtual const solution_maneuver_ref* GetVariants() const = 0;

      //! Получить индекс лучшего варианта безопасного расхождения
      virtual size_t GetBestVariantIndex() const = 0;
   };

   //! Кластер кораблей - множество внутри которого принимается кооперативное решение расхождения
   struct iCooperativeShipsCluster
   {
      //! Получить корабли в кластере с опасной траекторией движения
      virtual const danger_ship_info_ref* GetDangers() const = 0;

      //! Получить индификаторы кораблей кластера
      virtual const ids_ref* GetShipIds() const = 0;

      //! установить тип первого маневра при расхождении для корабля с заданным ID
      virtual bool SetPassingFirstManeuver(id_type shipId, FIRST_MANEUVER_TYPE type) = 0;    

      //! установить тип форсированного маневра расхождения для корабля с заданным ID
      virtual bool SetForceManeuver(id_type shipId, FORCE_MANEUVER_TYPE type) = 0;

      //! Получить манёвр корабля в кооперативном решении
      virtual const iCooperativeSolution* SolveCooperative() const = 0;

      //! Получить независимое решение для корабля
      virtual const iSingleSolution* SolveSingle(id_type shipId) const = 0;

      //! установить настройки для поиска решения
      virtual bool SetSettings(const settings& s) = 0;
   };

   using clusters_ref = base_ref<iCooperativeShipsCluster*>;

 
   //! Информация о результате обработки навигационной ситуации
   struct iNavEstimation : iReleasable
   {
      //! получить количество кораблей с опасной траекторией движения
      virtual size_t GetDangersCount()const = 0;

      //! получить время отценки, UTC
      virtual double GetEstimationTime()const = 0;

      //! получить кластеры опасностей
      virtual const clusters_ref* GetClusters() const = 0;

      //! получить предупреждения
      virtual const warnings_ref* GetWarnings() const = 0;

      //! Получить расширенный интерфейс для получения информации от STATE-FULL модели COLREG
      virtual const statefull::iStateFullNavEstimation* GetStateFullEstimation()const  = 0;
   };


   struct iColreg : iReleasable
   {
      virtual void SetCommunicator(ICommunicator* comm) = 0;

      //! Установить реализацию для интерфейса iChartSafetyCheck
      virtual void SetMapSafetyInterface(const iChartSafetyCheck* chartData) = 0;

      //! Установить реализацию для интерфейса iWeatherInfluence
      virtual void SetWeatherInfluenceInterface(const iWeatherInfluence* weather) = 0;

      //! Получить корневой элемент, который будет заполняться дебажной информацией
      virtual dbg::iDebugInfo* GetDebugInfo() = 0;

      //! Установить путь к папке ColregData
      virtual void SetColregDataPath(const colreg_paths& paths) = 0;

      /*!
      Настроить окружающую среду
      \param[in] points Район для настроек (nullptr - использовать везде)
      */
      virtual void SetEnvironment(const environment& env, const geo_point* points, size_t size) = 0;

      /*!
      Установить настройки colreg
      \param[in] points Район для настроек (nullptr - использовать везде)
      */
      virtual void SetSettings(const settings& s, const geo_point* points, size_t size) = 0;

      /*!
      Установить данные по кораблю
      \return Точка проекции позиции корабля на маршрут, с устаневленным временем нахождения на маршруте от его начала
      */
      virtual track_point_info SetShipData(const ship_info& info, const track_point_info& moveInfo) = 0;
      
      //! Установить ID корабля на котором работает colreg
      virtual void SetOwnShipID(id_type ownShipId) = 0;

      virtual void RemoveShip(id_type shipId) = 0;

      virtual void RemoveAllShips() = 0;

      //! Установить траекторию для корабля с заданным ID
      virtual bool SetPrediction(id_type shipId, const track_point_info* points, size_t size) = 0;

      //! Установить маршрут для корабля с заданным ID
      virtual bool SetRoute(id_type shipId, const route_point* points, size_t size, ROUTE_TYPE flag = ROUTE_TYPE::RT_MAIN ) = 0;

      //! Установить модель для корабля с заданным ID
      virtual void SetShipModelById(id_type shipId, const char* modelName) = 0;

      // ! Получить модель корабля с заданным ID
      virtual const char* GetShipModelNameById(id_type shipId) const = 0;

      //! Обработать навигационную обстановку
      virtual iNavEstimation* EstimateNavState(double time)  = 0;

      //! Получить модель корабля с заданным ID
      virtual const iModel* GetModel(id_type shipId) const = 0;

      //! Получить домен безопасности
      virtual iDomain* GetDomain(id_type shipId) const = 0;

      /*!
      Получить топологию домена безопасности
      \param[in] shipId Идентификатор корабля
      \param[in] time Сдвиг в секундах от текущей позиции
      */
      virtual const domain_geometry_ref* GetDomainTopology(id_type shipId, double time) const = 0;

      //! Получить расширенный интерфейс для взаимодействия с STATE-FULL моделью COLREG
      virtual statefull::iStateFullExtention* GetStateFullExtension() = 0;
   };

#pragma pack (pop)
}

#ifdef COLREGDLL_EXPORTS
#define EXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define EXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" EXPRTIMPRT colreg::iColreg* __cdecl CreateColreg(const colreg::colreg_paths* paths, const colreg::iModelManager* modelManager, colreg::COLREG_MODE colregMode);
extern "C" EXPRTIMPRT colreg::iColreg* __cdecl CreateRawColreg(const colreg::colreg_paths* paths, const colreg::iModelManager* modelManager, colreg::COLREG_MODE colregMode);