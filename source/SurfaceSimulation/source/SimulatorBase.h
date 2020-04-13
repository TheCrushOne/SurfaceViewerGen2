#pragma once

#include "common/communicator.h"
#include "colreg/ColregSimulation.h"
#include "colreg/ModuleGuard.h"
#include "crossdllinterface/SVGMDatabaseInterface.h"
#include "common/simulation_structs.h"
#include "crossdllinterface\ChartObjectGeneratorInterface.h"
#include "crossdllinterface\UniversalLoggerInterface.h"
#include "crossdllinterface\EngineInterface.h"

namespace ColregSimulation
{
   class SimulatorBase : public iSimulator, public Central
   {
   public:
      SimulatorBase(iPropertyInterface* prop)
         : m_prop(prop)
      {}

      virtual ~SimulatorBase() {}

      void Release() override final { delete this; }

      virtual bool Init(central_pack*);

      dbg::iDebugInfo* GetDebugInfo() const override final { return m_debugInfo; }
      virtual const std::shared_ptr<settings::application_settings>& GetAppSettings() const override final { return GetSettings(); }

      inline void SetSimulationType(const ColregSimulation::SIMULATION_PLAYER_TYPE type) { m_simulationType = type; }

      virtual SCENARIO_STATUS GetSimulatorScenarioState() const override final { return m_scenarioStatus; }
      virtual void SetSimulatorScenarioState(SCENARIO_STATUS status) override final { m_scenarioStatus = status; }
   protected:
      std::shared_ptr<file_utils::global_path_storage> m_paths;
      iPropertyInterface* m_prop;
      ColregSimulation::SIMULATION_PLAYER_TYPE m_simulationType = ColregSimulation::SIMULATION_PLAYER_TYPE::SPT_SIZE;
      scenario_data m_data;
      colreg::ModuleGuard<database::iSVGMDatabaseController> m_databaseController;
      colreg::ModuleGuard<engine::iEngine> m_engine;
      colreg::ModuleGuard<chart_object::iGenerator> m_generator;
      colreg::ModuleGuard<logger::iUniversalLogger> m_logger;
      SCENARIO_STATUS m_scenarioStatus = SCENARIO_STATUS::SS_NOT_LOADED;
      dbg::iDebugInfo* m_debugInfo = nullptr;
   };
}