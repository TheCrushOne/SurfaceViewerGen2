#pragma once

#include "common/communicator.h"
#include "colreg/ColregSimulation.h"
#include "colreg/ModuleGuard.h"
#include "crossdllinterface/SVGMDatabaseInterface.h"
#include "common/simulation_structs.h"
#include "crossdllinterface\UniversalLoggerInterface.h"
#include "crossdllinterface\EngineInterface.h"
#include "crossdllinterface\TaskInterface.h"

#include "datastandart/SVGenMapDataStandartInterface.h"
#include "datastandart/PathStorageDataStandartInterface.h"
#include "datastandart/OptimizedPathStorageDataStandartInterface.h"

#include "navdisp\OrderBase.h"

namespace ColregSimulation
{
   class SimulatorBase : public iSimulator, public Central
   {
   public:
      SimulatorBase(central_pack*, iPropertyInterface*, navigation_dispatcher::iComServicePtr);

      virtual ~SimulatorBase() {}

      void Release() override final { delete this; }

      dbg::iDebugInfo* GetDebugInfo() const override final { return m_debugInfo; }
      bool CheckOpenScenario() override;
      bool LoadProcessedMap() override;
      bool LoadProcessedPaths() override;
      bool LoadProcessedOptPaths() override;
      //virtual const std::shared_ptr<settings::application_settings>& GetAppSettings() const override final { return GetSettings(); }

      inline void SetSimulationType(const ColregSimulation::SIMULATION_PLAYER_TYPE type) { m_simulationType = type; }
      void SetAppSettings(const settings::application_settings& settings) override final { m_settings = settings; }
      const settings::application_settings& GetAppSettings() const override final { return m_settings; }

      virtual SCENARIO_STATUS GetSimulatorScenarioState() const override final { return m_scenarioStatus; }
      virtual void SetSimulatorScenarioState(SCENARIO_STATUS status) override final { m_scenarioStatus = status; }
   protected:
      settings::application_settings m_settings;
      std::shared_ptr<file_utils::global_path_storage> m_paths;
      iPropertyInterface* m_prop;

      ColregSimulation::SIMULATION_PLAYER_TYPE m_simulationType = ColregSimulation::SIMULATION_PLAYER_TYPE::SPT_SIZE;
      scenario_data m_data;
      colreg::ModuleGuard<database::iSVGMDatabaseController, central_pack*> m_databaseController;
      colreg::ModuleGuard<engine::iEngine, central_pack*> m_engine;
      colreg::ModuleGuard<logger::iUniversalLogger, central_pack*> m_logger;

      colreg::ModuleGuard<data_standart::iDataStandart, central_pack_ptr, LPCSTR, navigation_dispatcher::iComService*> m_mapDS;
      colreg::ModuleGuard<data_standart::iDataStandart, central_pack_ptr, LPCSTR, navigation_dispatcher::iComService*> m_pathDS;
      colreg::ModuleGuard<data_standart::iDataStandart, central_pack_ptr, LPCSTR, navigation_dispatcher::iComService*> m_optPathDS;
      SCENARIO_STATUS m_scenarioStatus = SCENARIO_STATUS::SS_NOT_LOADED;
      dbg::iDebugInfo* m_debugInfo = nullptr;
      navigation_dispatcher::iComServicePtr m_service = nullptr;
   };
}