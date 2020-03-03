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
      RobotLogPlayer(ICommunicator* pCommunicator, iPropertyInterface* prop);
      ~RobotLogPlayer() = default;

      // iSimulator impl
      void Start() override;
      void Stop() override;
      void Reset() override;
      bool IsRunning() const override;
      size_t GetControlPointsCount() const override;
      const control_point_info& GetControlPointInfo(size_t controlPntIdx) const override;
      bool PlayFrom(size_t controlPointIdx) override;
      bool NextControlPoint() override;
      size_t GetCurrentControlPointIdx() const override;
      const iSimulationState& GetState() const override { return *this; }
      bool SaveLogPair(const char* filename) const override;
      const ColregSimulation::SIMULATION_PLAYER_TYPE GetSimulationType() override;
      void SetAppSettings(const settings::application_settings& s) override;
      void ReloadSettings() override;
      const settings::application_settings& GetAppSettings() const override;

      // iSimulationState impl
      size_t GetUnitCount(UNIT_TYPE type) const;
      const iUnit& GetUnit(UNIT_TYPE type, size_t idx) const;
      double GetTime() const;
      bool PrepareDataForSave(const ScenarioIO::scenario_data* pInputScenarioData, ScenarioIO::scenario_data* pScenarioData, const bool focused, const colreg::geo_points_ref& ships, const colreg::base_ref<colreg::geo_points_ref>& chart_objects) const;
   };
}