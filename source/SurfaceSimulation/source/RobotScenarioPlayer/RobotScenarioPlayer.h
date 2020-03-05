#pragma once

#include "SimulatorBase.h"
#include "crossdllinterface\EngineInterface.h"
#include "common\pathfinder_structs.h"
#include "SimulationModel/ColregSimulationImpl.h"

namespace ColregSimulation
{
   class RobotScenarioPlayer
      : public iSimulationState
      , public SimulatorBase
   {
   public:
      RobotScenarioPlayer(ICommunicator* pCommunicator, iPropertyInterface* prop);
      ~RobotScenarioPlayer() = default;

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
      bool PrepareDataForSave(/*const ScenarioIO::scenario_data* pInputScenarioData, ScenarioIO::scenario_data* pScenarioData, */const bool focused, const colreg::geo_points_ref& ships, const colreg::base_ref<colreg::geo_points_ref>& chart_objects) const;
   protected:
      void prepareRootData();
      void updateUnitsPath();
      void addUnit(const settings::point_setting_element&, UNIT_TYPE);
      void addUnitsFromScenario();
   private:
      std::shared_ptr<pathfinder::route_data> m_routedata;
      std::vector<std::vector<double>> m_coordGrid;
      colreg::ModuleGuard<engine::iEngine> m_engine;

      std::vector<SimulationShip> m_ships;
      std::vector<SimulationRover> m_rovers;
      std::vector<SimulationDrone> m_drones;
   };
}