#pragma once

#include "ColregInterfaces.h"
#include "WeatherInterface.h"
#include "StateFullColregInterface.h"
#include "common/settings.h"
#include "common/file_storage.h"

struct iPropertyInterface;

namespace ScenarioIO
{
   struct scenario_data;
}

namespace ColregSimulation
{
#pragma pack (push, 4)
   //!тип маршрута
   enum class ROUTE_TYPE : char
   {
      RT_COLREG = 0,    //< Для передачив COLREG
      RT_SIMULAION,     //< Для симуляции движения
      RT_SOURSE,        //< Исходный маршрут движения из сценария
      RT_DISIGION,      //< Маршрут-решение
      RT_SUB_OPTIMAL,   //< Маршрут-решение
   };

   struct control_point_info
   {
      double time;
   };

   //! Полная информация для точки трека
   struct track_point_full_info
   {
      track_point_full_info()
      {}

      track_point_full_info(const colreg::track_point_info& pt)
         : point{ pt }
      {}

      track_point_full_info(const colreg::track_point_info& pt, const colreg::model_point_info& modelInfo)
         : point{ pt }
         , model_info{ modelInfo }
      {}

      colreg::track_point_info   point;
      colreg::model_point_info   model_info;
      colreg::chart_context      chart_context;
      colreg::domain_border_info domain_border;
   };

   //! Данные пути корабля
   struct ship_path_ref
   {
      ship_path_ref() = default;

      const colreg::route_ref*           route = nullptr;
      const colreg::track_full_info_ref* track = nullptr;
   };

   struct simulation_ship_settings
   {
      bool no_accept_route = false; ///< Игнорировать предложенный маршурт
      bool ignore_route_suggestions = true; ///< Игнорировать рекомендации по смене маршрута

      bool move_with_custom_params = false;       ///< Игнорировать модельную траекторию и двигаться по параметрам movement_course и movement_speed
      double movement_course  = colreg::NO_VALUE; ///< Курс, по которому двигается судно при игнорировании траектории (NO_VALUE == остается текущим)
      double movement_heading = colreg::NO_VALUE; ///< Направление носа судна при игнорировании траектории (NO_VALUE == остается текущим)
      double movement_speed   = colreg::NO_VALUE; ///< Скорость, с которой двигается судно при игнорировании траектории (NO_VALUE == остается текущим)

      bool calculate_chart_context = false; ///< Просчет контекста карты для модельной траектории корабля (если выкл, то контекст по-умолчанию)
      bool calculate_domain_border = false; ///< Просчет коридора домена для модельной траектории корабля (если выкл, то контекст по-умолчанию)

      double offset_ship_pos_from_route_left = 0.; ///< Отступ текущей позиции корабля перпендикулярно маршруту, мили
      double offset_ship_pos_from_route_right = 0.; ///< Отступ текущей позиции корабля перпендикулярно маршруту, мили
   };

   //! Интерфейс доступа к данным корабля
   struct iShip
   {
      //! Информационные данные корабля
      virtual colreg::ship_info GetInfo() const = 0;

      virtual const simulation_ship_settings& GetSimulationSettings() const = 0;

      virtual const colreg::domain_scales& GetDomainScales() const = 0;

      //! Полная информация по текущему местоположению
      virtual track_point_full_info GetPos() const = 0;

      virtual const ship_path_ref* GetRoute(ROUTE_TYPE type)const = 0;
      
      //! Исходный путь
      virtual const ship_path_ref* GetSrcPath() const = 0;

      //! Симулированный путь
      virtual const ship_path_ref* GetSimulationPath() const = 0;

      //! Симулированный путь
      virtual const ship_path_ref* GetPredictionPath() const = 0;

      //! Путь из модели colreg
      virtual const ship_path_ref* GetModelPath() const = 0;

      //! Получить проекцию текущей позиции на маршрут
      virtual colreg::track_point_info GetProjectionToRoute() const = 0;

      /*! Получить топологию домена корабля в момент времени
      \param[in] scales Получить топологию с переданными коэффициентами сжатия 
      */
      virtual const colreg::domain_geometry_ref* GetDomainTopology(double time, const colreg::domain_scales* scales = nullptr) const = 0;
    
      virtual const colreg::iVesselMathModel* GetMathModel()const = 0;
      virtual bool IsRouteExtended()const { return false; }

      //для AIS Log должно возвращаться true
      virtual bool IsReadOnly() const = 0; 

      virtual const char* GetDestination() const = 0;
      virtual const char* GetETA() const = 0;

      virtual double GetColregRouteZoneWidth(unsigned int routeIndex, bool left, colreg::ROUTE_ZONE_TYPE zone) const = 0;
      virtual const colreg::iModel* GetSourceRouteModel() const = 0;
      virtual ~iShip() = default;
   };

   struct iSimulationEstimation
   {
      virtual bool Solve(bool isCooperative) const = 0;

      virtual const colreg::clusters_ref* GetClusters() const = 0;
      virtual size_t GetSolutionCount(size_t clusterIdx) const = 0;

      virtual const colreg::danger_ship_info* GetDangerInfoById(colreg::id_type shipId) const = 0;
      virtual const colreg::danger_ship_info* GetDangerInfoById(size_t clusterIdx, colreg::id_type shipId) const = 0;

      virtual const colreg::iSingleSolution* GetSolution(size_t clusterIdx, size_t solIdx) const = 0;
      virtual const colreg::iSingleSolution* GetSolutionById(size_t clusterIdx, colreg::id_type shipId) const = 0;

      virtual const colreg::iNavEstimation* GetResult() const = 0;
   };

#pragma pack (push, 1)
   struct simulation_settings
   {
      colreg::id_type ownship_id;      // Colreg работает для корабля self
      double simulationTimeStep;       // интервал step()
      double estimationInterval;       // интервал для Estimate
      bool auto_apply;                 // авто принятие решения COLREG
      bool extend_route_last_point;    // автоматически продлевать маршруты из сценария
      bool scripted_scenario;          // является ли сценарий скриптовым
      size_t stepCount;                // количество шагов сценария      
      simulation_settings()
         : ownship_id{ colreg::INVALID_ID }
         , simulationTimeStep{ 60 }
         , estimationInterval{ 60 }
         , auto_apply{true}
         , extend_route_last_point{true}
         , scripted_scenario{false}
         , stepCount{ 0 }
      {}
   };
#pragma pack (pop)
   //! Срез состояния симуляции
   struct iSimulationState
   {
      /*!
      Сохранение данных в файл
      \param[in] scenarioPath Путь по которому необходимо сохранить
      */
      virtual bool Save(const char* scenarioPath, const char* mapFolderPath, const bool focused, const colreg::geo_points_ref& ships, const colreg::base_ref<colreg::geo_points_ref>& chart_objects) const = 0;

      /*!
      Сохранение настроек в файл рядом со сценарием
      \param[in] scenarioPath Путь по которому лежит сценарий сохранить
      */
      virtual bool SaveSettings(const char* scenarioPath) const = 0;

      virtual size_t       GetShipCount()      const = 0;
      virtual const iShip& GetShip(size_t idx) const = 0;

      //! Абсолютное время (UTC) среза симуляции
      virtual double GetTime() const = 0;

      //! Текущая навигационная оценка с решениями
      virtual const iSimulationEstimation* GetEstimation() const = 0;

      virtual const colreg::iChartSafetyCheck* GetSafetyChecker() const = 0;

      virtual const colreg::settings& GetSettings() const = 0;

      virtual const colreg::environment& GetEnvironment() const = 0;

      virtual const ColregSimulation::simulation_settings& GetSimulationSettings() const = 0;

      virtual const colreg::weather_influence_info& GetWeatherSettings() const = 0;

      virtual colreg::id_type OwnShipId() const = 0;

      virtual bool PrepareDataForSave(const ScenarioIO::scenario_data* pInputScenarioData, ScenarioIO::scenario_data* pScenarioData, const bool focused, const colreg::geo_points_ref& ships, const colreg::base_ref<colreg::geo_points_ref>& chart_objects) const = 0;

      virtual ~iSimulationState() = default;
   };

   enum ActionType
   {
      AT_ACCEPTMAN,
      AT_CANCELMAN
   };

   enum class SIMULATION_PLAYER_TYPE : char
   {
      SPT_SCENARIO = 0,
      SPT_LOG,
      SPT_SIZE
   };

   struct ActionData
   {
      colreg::id_type id;
      double time;
      ActionType actType;
      bool isAction;
      colreg::MANEUVER_TYPE manType;
   };

   //! Интерфейс симулятора колрега
   struct iSimulator : colreg::iReleasable
   {
      //! Запуск симулятора
      virtual void Start() = 0;

      //! Остановка симуляции, сброс данных о кораблях в исходное положение
      virtual void Stop() = 0;

      //! Остановка симуляции, сброс всех данных без выгрузки dll
      virtual void Reset() = 0;

      //! Запущен ли симулятор
      virtual bool IsRunning() const = 0;

      //! Количество контрольных точек (состояний) симулятора
      virtual size_t GetControlPointsCount() const = 0;

      //! Получить контрольную точку по индексу
      virtual const control_point_info& GetControlPointInfo(size_t controlPntIdx) const = 0;

      //! Начать проигрывание с контрольной точки
      virtual bool PlayFrom(size_t controlPointIdx) = 0;

      //! Шаг к следующей контрольной точке
      virtual bool NextControlPoint() = 0;

      //! Индекс текущей контрольной точка
      virtual size_t GetCurrentControlPointIdx() const = 0;

      //! Получить доступ к текущему срезу состояния симулятора
      //virtual const iSimulationState& GetState() const = 0;

      //! Сохранение комбинации лога и карты
      virtual bool SaveLogPair(const char* filename) const = 0;

      //! Получить тип симуляции
      virtual const ColregSimulation::SIMULATION_PLAYER_TYPE GetSimulationType() = 0;

      //! Установить настройки
      virtual void SetSettings(const settings::application_settings& s) = 0;
      
      //! Перезагрузить настройки с xml
      virtual void ReloadSettings() = 0;

      //! Взять настройки
      virtual const settings::application_settings& GetSettings() const = 0;
      
      /*!
      \brief Добавить динамические объекты карты в симуляцию
      ID назначаются при запросе из GetState (id в objects игнорятся)
      */
      //virtual colreg::chart_object_id AddDynamicChartObjects(const colreg::chart_object* objects, size_t size) = 0;

      /*!
      \brief Добавить статические объекты карты в симуляцию
      После добавления запускается сборка данных карты.
      ID назначаются при запросе из GetState (id в objects игнорятся)
      */
      //virtual colreg::chart_object_id AddStaticChartObjects(const colreg::chart_object* objects, size_t size) = 0;

      //! Убрать объекты карты из симуляции
      //virtual bool RemoveChartObjects(const colreg::chart_object_id* ids, size_t size) = 0;

      /*!
      \brief Задать новую геометрию и свойства объектам карты
      После добавления запускается сборка данных карты.
      ID назначаются при запросе из GetState (id в data игнорятся)
      */
      //virtual bool SetChartObjectsData(const colreg::chart_object_id* ids, const colreg::chart_object* data, size_t size) = 0;

      ///////////////////////////statefull//////////////////////////////////////////////////

      //virtual void ReEstimate() = 0;

      //virtual const char* GetSimulationTimeStr() const = 0;
   };

   struct simulation_paths
   {
      const char* scenario_path = nullptr; ///< Путь к сценарию (png, svgm)
      const char* scenario_result_path = nullptr; ///< Путь по которому будут складываться данные сценария
   };


   // отвечает за чтение и обработку данных кораблей из лога
   // участвует в проигрывании логов
   struct iLogProcessor
   {
      virtual double GetFirstRecordTime() const = 0; 
      virtual double GetCurrentRecordTime() const = 0;
      virtual double GetLastRecordTime() const = 0;
      virtual void Reset() = 0;
      
      //передвинуть корабли вперед на deltaSeconds секунд вперед
      // если continueOverLast = true, Двигает вперед даже если зашли за конец лога, в соответствии с последним
      // курсом корабля
      // возвращает false, если зашли за последнюю запись и continueOverLast = false
      virtual bool MoveShips(double deltaSeconds, bool continueOverLast = false ) = 0;
      virtual size_t       GetShipCount()      const = 0;
      virtual const iShip* GetShip(size_t idx) const = 0;
      virtual const iShip* GetShipById(colreg::id_type shipId) const = 0;
      virtual ~iLogProcessor() = default;
   };

   using LogProcessorPtr = std::unique_ptr< iLogProcessor >;
#pragma pack(pop)
}