#pragma once

#include "ColregInterfaces.h"
#include "WeatherInterface.h"
#include "StateFullColregInterface.h"
#include "common/settings.h"
#include "common/file_storage.h"

struct iPropertyInterface;

namespace ColregSimulation
{
#pragma pack (push, 4)
   //!тип маршрута
   enum class ROUTE_TYPE : char
   {
      RT_COLREG = 0,    //< Для передачи в COLREG/pathfinder
      RT_SIMULAION,     //< Для симуляции движения
      RT_SOURSE,        //< Исходный маршрут движения из сценария
      RT_DISIGION,      //< Маршрут-решение
      RT_SUB_OPTIMAL,   //< Маршрут-решение
   };

   enum class UNIT_TYPE : char
   {
      UT_ROVER = 0,
      UT_DRONE,
      UT_SHIP,
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
      colreg::domain_border_info domain_border;
   };

   //! Данные пути корабля
   struct ship_path_ref
   {
      ship_path_ref() = default;

      const colreg::route_ref*           route = nullptr;
      const colreg::track_full_info_ref* track = nullptr;
   };

   struct iUnit
   {
      //! Информационные данные юнита
      virtual colreg::ship_info GetInfo() const = 0;

      virtual const colreg::domain_scales& GetDomainScales() const = 0;

      //! Полная информация по текущему местоположению
      virtual track_point_full_info GetPos() const = 0;

      //! Псевдосписок путей
      virtual const ship_path_ref* GetRoute(ROUTE_TYPE type)const = 0;

      //! Исходный путь
      virtual const ship_path_ref* GetSrcPath() const = 0;

      //! Симулированный путь
      virtual const ship_path_ref* GetSimulationPath() const = 0;

      //! Симулированный путь
      virtual const ship_path_ref* GetPredictionPath() const = 0;

      /*! Получить топологию домена юнита в момент времени
      \param[in] scales Получить топологию с переданными коэффициентами сжатия
      */
      virtual const colreg::domain_geometry_ref* GetDomainTopology(double time, const colreg::domain_scales* scales = nullptr) const = 0;

      virtual const char* GetETA() const = 0;

      virtual ~iUnit() = default;
   };

   //! Интерфейс доступа к данным ровера
   struct iRover
   {};
    
   //! Интерфейс доступа к данным дрона
   struct iDrone
   {};

   //! Интерфейс доступа к данным корабля
   struct iShip
   {
      //virtual const simulation_ship_settings& GetSimulationSettings() const = 0;
  
      //! Путь из модели colreg
      //virtual const ship_path_ref* GetModelPath() const = 0;

      //! Получить проекцию текущей позиции на маршрут
      //virtual colreg::track_point_info GetProjectionToRoute() const = 0;
    
      //virtual const colreg::iVesselMathModel* GetMathModel()const = 0;
      virtual bool IsRouteExtended() const { return false; }

      //для AIS Log должно возвращаться true
      //virtual bool IsReadOnly() const = 0; 

      //virtual const char* GetDestination() const = 0;
      //virtual const char* GetETA() const = 0;

      //virtual double GetColregRouteZoneWidth(unsigned int routeIndex, bool left, colreg::ROUTE_ZONE_TYPE zone) const = 0;
      //virtual const colreg::iModel* GetSourceRouteModel() const = 0;
      virtual ~iShip() = default;
   };

   //! Срез состояния симуляции
   struct iSimulationState
   {
      /*!
      Сохранение данных в файл
      \param[in] scenarioPath Путь по которому необходимо сохранить
      */
      //virtual bool Save(const char* scenarioPath, const char* mapFolderPath, const bool focused, const colreg::geo_points_ref& ships, const colreg::base_ref<colreg::geo_points_ref>& chart_objects) const = 0;

      /*!
      Сохранение настроек в файл рядом со сценарием
      \param[in] scenarioPath Путь по которому лежит сценарий сохранить
      */
      //virtual bool SaveSettings(const char* scenarioPath) const = 0;

      virtual size_t       GetUnitCount(UNIT_TYPE type)      const = 0;

      virtual const iUnit& GetUnit(UNIT_TYPE type, size_t idx) const = 0;

      virtual const colreg::chart_objects_ref& GetChartObjects() const = 0;

      //! Абсолютное время (UTC) среза симуляции
      virtual double GetTime() const = 0;

      //! Текущая навигационная оценка с решениями
      //virtual const iSimulationEstimation* GetEstimation() const = 0;

      //virtual const colreg::iChartSafetyCheck* GetSafetyChecker() const = 0;

      //virtual const colreg::settings& GetSettings() const = 0;

      //virtual const colreg::environment& GetEnvironment() const = 0;

      //virtual const ColregSimulation::simulation_settings& GetSimulationSettings() const = 0;

      //virtual const colreg::weather_influence_info& GetWeatherSettings() const = 0;

      //virtual colreg::id_type OwnShipId() const = 0;

      virtual bool PrepareDataForSave(/*const ScenarioIO::scenario_data* pInputScenarioData, ScenarioIO::scenario_data* pScenarioData, */const bool focused, const colreg::geo_points_ref& ships, const colreg::base_ref<colreg::geo_points_ref>& chart_objects) const = 0;

      virtual ~iSimulationState() = default;
   };

   enum class SIMULATION_PLAYER_TYPE : char
   {
      SPT_SCENARIO = 0,
      SPT_LOG,
      SPT_SIZE
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
      virtual const iSimulationState& GetState() const = 0;

      //! Сохранение комбинации лога и карты
      virtual bool SaveLogPair(const char* filename) const = 0;

      //! Получить тип симуляции
      virtual const ColregSimulation::SIMULATION_PLAYER_TYPE GetSimulationType() = 0;

      //! Установить настройки
      virtual void SetAppSettings(const settings::application_settings& s) = 0;
      
      //! Перезагрузить настройки с xml
      virtual void ReloadSettings() = 0;

      //! Взять настройки
      virtual const settings::application_settings& GetAppSettings() const = 0;
      
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
#pragma pack(pop)
}