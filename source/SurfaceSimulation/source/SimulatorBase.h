#pragma once

#include "common/communicator.h"
#include "colreg/ColregSimulation.h"
#include "colreg/ModuleGuard.h"
#include "crossdllinterface/SVGMDatabaseInterface.h"
#include "common/simulation_structs.h"
#include "common/chart_object.h"
#include "crossdllinterface\UniversalLoggerInterface.h"
#include "crossdllinterface\EngineInterface.h"
#include "crossdllinterface\TaskInterface.h"

#include "datastandart/SVGenMapDataStandartInterface.h"
#include "datastandart/PathStorageDataStandartInterface.h"
#include "datastandart/OptimizedPathStorageDataStandartInterface.h"
#include "datastandart/ChartObjectDataStandartInterface.h"

#include "navdisp\OrderBase.h"

namespace ColregSimulation
{
   class SimulatorBase
      : public iSimulator
      , public Central
   {
      struct tag
      {
         // TODO: запаковать в одно место(дублируется с ConfigDispatcherImpl.h)
         static constexpr char root[] = "root";
         static constexpr char standarts[] = "data_standarts";
         static constexpr char standart[] = "data_standart";
         static constexpr char type[] = "type";
         static constexpr char params[] = "params";
      };
   public:
      SimulatorBase(central_pack_ptr, iPropertyInterface*, navigation_dispatcher::iComServicePtr);

      virtual ~SimulatorBase() {}

      void Release() override final { delete this; }

      //dbg::iDebugInfo* GetDebugInfo() const override final { return m_debugInfo; }
      bool CheckOpenScenario() override;
      bool LoadProcessedMap() override;
      bool LoadProcessedMapObjects() override;
      bool LoadProcessedPaths() override;
      bool LoadProcessedOptPaths() override;
      //virtual const std::shared_ptr<settings::application_settings>& GetAppSettings() const override final { return GetSettings(); }

      inline void SetSimulationType(const ColregSimulation::SIMULATION_PLAYER_TYPE type) { m_simulationType = type; }
      void SetAppSettings(const settings::application_settings& settings) override final { m_settings = settings; }
      const settings::application_settings& GetAppSettings() const override final { return m_settings; }

      virtual SCENARIO_STATUS GetSimulatorScenarioState() const override final { return m_scenarioStatus; }
      virtual SIMULATION_STATUS GetSimulatorSimulationState() const override final { return m_simulationStatus; }
      virtual void SetSimulatorScenarioState(SCENARIO_STATUS status) override final { m_scenarioStatus = status; }
      virtual void SetSimulatorSimulationState(SIMULATION_STATUS status) override final { m_simulationStatus = status; }
   protected:
      //void deserializeStandartAttrs()
   protected:
      settings::application_settings m_settings;
      chart_object::chart_object_unit_vct m_chartObjects;
      std::shared_ptr<file_utils::global_path_storage> m_paths;
      iPropertyInterface* m_prop;
      std::wstring m_orderCacheFolder, m_currentConfig;

      ColregSimulation::SIMULATION_PLAYER_TYPE m_simulationType = ColregSimulation::SIMULATION_PLAYER_TYPE::SPT_SIZE;
      scenario_data m_data;
      colreg::ModuleGuard<database::iSVGMDatabaseController, central_pack*> m_databaseController;
      colreg::ModuleGuard<engine::iEngine, central_pack_ptr> m_engine;
      colreg::ModuleGuard<logger::iUniversalLogger, central_pack_ptr> m_logger;

      colreg::ModuleGuard<data_standart::iDataStandart, central_pack_ptr, LPCSTR, navigation_dispatcher::iComService*> m_mapDS;
      colreg::ModuleGuard<data_standart::iDataStandart, central_pack_ptr, LPCSTR, navigation_dispatcher::iComService*> m_mapObjDS;
      colreg::ModuleGuard<data_standart::iDataStandart, central_pack_ptr, LPCSTR, navigation_dispatcher::iComService*> m_pathDS;
      colreg::ModuleGuard<data_standart::iDataStandart, central_pack_ptr, LPCSTR, navigation_dispatcher::iComService*> m_optPathDS;
      SCENARIO_STATUS m_scenarioStatus = SCENARIO_STATUS::SS_NOT_LOADED;
      SIMULATION_STATUS m_simulationStatus = SIMULATION_STATUS::SS_STOP;
      //dbg::iDebugInfo* m_debugInfo = nullptr;
      navigation_dispatcher::iComServicePtr m_service = nullptr;
   };
}