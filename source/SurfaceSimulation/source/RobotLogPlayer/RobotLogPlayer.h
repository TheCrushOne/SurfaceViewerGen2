#pragma once

#include "SimulatorBase.h"
#include "SimulationModel/ColregSimulationImpl.h"

namespace ColregSimulation
{
   class RobotLogPlayer
      : public iSimulationState
      , public SimulatorBase
   {
   public:
      RobotLogPlayer(central_pack*, iPropertyInterface*, navigation_dispatcher::iComServicePtr);
      ~RobotLogPlayer() = default;

      // iSimulator impl
      void Start() override final;
      void Stop() override final;
      void Reset() override final;
      bool IsRunning() const override final;
      size_t GetControlPointsCount() const override final;
      const control_point_info& GetControlPointInfo(size_t controlPntIdx) const override final;
      bool PlayFrom(size_t controlPointIdx) override final;
      bool NextControlPoint() override final;
      size_t GetCurrentControlPointIdx() const override final;
      const iSimulationState& GetState() const override final { return *this; }
      bool SaveLogPair(const char* filename) const override final;
      const ColregSimulation::SIMULATION_PLAYER_TYPE GetSimulationType() override final;
      void ReloadSettings() override final;
      void RecountRoutes() override final;
      void RecountResearch() override final;
      void LogResearchResult() override final;

      // iSimulationState impl
      size_t GetUnitCount(UNIT_TYPE type) const override final;
      const iUnit* GetUnitByIdx(UNIT_TYPE type, size_t idx) const override final;
      const iUnit* GetUnitById(colreg::id_type id) const override final;
      double GetTime() const override final;
      const settings::map_settings& GetChartGridMeta() const override final;
      const chart_object::chart_object_unit_vct_ref GetChartObjects() const override final;
      const chart_object::chart_object_unit* GetChartObject(colreg::chart_object_id id) const override final;
      bool PrepareDataForSave(const bool focused, const colreg::geo_points_vct_ref ships, const chart_object::chart_object_unit_vct_ref chart_objects) const override final;
   private:
      chart_object::chart_object_unit_vct m_chartObjects;
   };
}