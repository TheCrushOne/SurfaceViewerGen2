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
      RobotScenarioPlayer(central_pack*, iPropertyInterface*, navigation_dispatcher::iComServicePtr);
      ~RobotScenarioPlayer() = default;

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
      const settings::unit_data_element& GetUnitByIdx(UNIT_TYPE type, size_t idx) const override final;
      const iUnit* GetUnitById(SVCG::id_type id) const override final;
      double GetTime() const override final;
      const settings::map_settings& GetChartGridMeta() const override final;
      const chart_object::chart_object_unit_vct& GetChartObjects() const override final;
      const chart_object::chart_object_unit* GetChartObject(SVCG::chart_object_id id) const override final;
      bool PrepareDataForSave(const bool focused, const SVCG::geo_contour_vct& ships, const chart_object::chart_object_unit_vct& chart_objects) const override final;
   protected:
      settings::unit_data_element& getUnitByIdx(UNIT_TYPE type, size_t idx);
      void start(std::function<void(void)>);

      void step();
   private:
      std::vector<std::vector<double>> m_coordGrid;

      chart_grid_meta m_gridMeta = {};

      //size_t m_maxStepIdx;

      
   };
}