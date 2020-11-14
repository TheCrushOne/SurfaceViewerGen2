#pragma once

#include "layer_provider/layer_unit_object.h"
#include "layer_provider/layer_chart_object.h"
#include "colreg/SimulationTypes.h"
#include "simulation/LayerUnitInterface.h"
#include "simulation/LayerChartObjectInterface.h"

namespace SV::surface_simulation
{
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

      //virtual SVCG::trajectory_point& GetCurrentUnitPositionByIdx(UNIT_TYPE type, size_t idx) const = 0;

      //! Unit dee
      virtual size_t GetUnitCount(UNIT_TYPE type) const = 0;

      virtual const iLayerUnit* GetUnitByIdx(UNIT_TYPE type, size_t idx) const = 0;

      virtual const iLayerUnit* GetUnitById(id_type id) const = 0;

      //! Chart object dee
      virtual size_t GetChartObjectCount() const = 0;

      virtual const iLayerChartObject* GetChartObjectByIdx(size_t idx) const = 0;

      virtual const iLayerChartObject* GetChartObjectById(chart_object_id id) const = 0;

      //! Абсолютное время (UTC) среза симуляции
      virtual double GetTime() const = 0;

      virtual const settings::map_settings& GetChartGridMeta() const = 0;

      //! Explication dee
      virtual const pathfinder::UnsignedMatrix& GetLandUnitExplication() const = 0;

      virtual const pathfinder::UnsignedMatrix& GetAirUnitExplication() const = 0;

      virtual const std::vector<pathfinder::UnsignedMatrix>& GetCoverageHistory() const = 0;

      //! Текущая навигационная оценка с решениями
      //virtual const iSimulationEstimation* GetEstimation() const = 0;

      //virtual const colreg::settings& GetSettings() const = 0;

      //virtual const colreg::environment& GetEnvironment() const = 0;

      //virtual const surface_simulation::simulation_settings& GetSimulationSettings() const = 0;

      //virtual const colreg::weather_influence_info& GetWeatherSettings() const = 0;

      //virtual colreg::id_type OwnShipId() const = 0;

      virtual bool PrepareDataForSave(const bool focused, const CG::geo_contour_vct& ships, const chart_object::chart_object_unit_vct& chart_objects) const = 0;

      virtual ~iSimulationState() = default;
   };
}