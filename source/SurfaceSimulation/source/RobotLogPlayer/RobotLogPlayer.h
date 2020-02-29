#pragma once

#include "SimulatorBase.h"

namespace ColregSimulation
{
   class RobotLogPlayer
      : public SimulatorBase
   {
   public:
      RobotLogPlayer(ICommunicator* pCommunicator, iPropertyInterface* prop);
      ~RobotLogPlayer() = default;

      void Start() override;
      void Stop() override;
      void Reset() override;
      bool IsRunning() const override;
      size_t GetControlPointsCount() const override;
      const control_point_info& GetControlPointInfo(size_t controlPntIdx) const override;
      bool PlayFrom(size_t controlPointIdx) override;
      bool NextControlPoint() override;
      size_t GetCurrentControlPointIdx() const override;
      bool SaveLogPair(const char* filename) const override;
      const ColregSimulation::SIMULATION_PLAYER_TYPE GetSimulationType() override;
      void SetSettings(const settings::application_settings& s) override;
      void ReloadSettings() override;
      const settings::application_settings& GetSettings() const override;
   };
}