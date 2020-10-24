#pragma once
#include "BaseRef.h"
#include "SimulationStateInterface.h"

namespace SV::surface_simulation
{
   //! Интерфейс симулятора
   struct iSimulator : iReleasable
   {
      //! Получить корневой элемент, который будет заполняться дебажной информацией
      //virtual dbg::iDebugInfo* GetDebugInfo() const = 0;

      //! Реакция на загрузку метаданных сценария
      virtual bool CheckOpenScenario() = 0;

      //! Загрузить обработанную карту высот
      virtual bool LoadProcessedMap() = 0;

      //! Загрузить сгенерированные объекты карты
      virtual bool LoadProcessedMapObjects() = 0;

      //! Загрузить рассчитанный стандартный путь
      virtual bool LoadProcessedPaths() = 0;

      //! Загрузить рассчитанные оптимизированные пути
      virtual bool LoadProcessedOptPaths() = 0;

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
      //virtual const control_point_info& GetControlPointInfo(size_t controlPntIdx) const = 0;

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
      virtual const surface_simulation::SIMULATION_PLAYER_TYPE GetSimulationType() = 0;

      //! Установить настройки
      virtual void SetAppSettings(const settings::application_settings& s) = 0;

      //! Перезагрузить настройки с xml
      virtual void ReloadSettings() = 0;

      //! Взять настройки
      virtual const settings::application_settings& GetAppSettings() const = 0;

      //! Перерасчет путей
      virtual void RecountRoutes() = 0;

      //! Перерасчет исследования
      virtual void RecountResearch() = 0;

      //! Логгирование результатов исследования
      virtual void LogResearchResult() = 0;

      //! Статус сценария
      virtual SCENARIO_STATUS GetSimulatorScenarioState() const = 0;

      //! Статус симуляции
      virtual SIMULATION_STATUS GetSimulatorSimulationState() const = 0;

      //! Установка статуса сценария
      virtual void SetSimulatorScenarioState(SCENARIO_STATUS) = 0;

      //! Установка статуса симуляции
      virtual void SetSimulatorSimulationState(SIMULATION_STATUS) = 0;

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
}