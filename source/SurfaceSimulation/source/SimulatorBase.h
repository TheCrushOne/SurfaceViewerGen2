#pragma once

#include "common/central_class.h"
#include "common/servicable.h"
#include "common/communicator.h"
//#include "colreg/ColregSimulation.h"
#include "colreg/PropertyInterface.h"
#include "colreg/ModuleGuard.h"
#include "colreg/SimulationStateInterface.h"
#include "colreg/SimulatorInterface.h"
#include "crossdllinterface/SVGMDatabaseInterface.h"
#include "common/chart_object_unit.h"
#include "crossdllinterface\UniversalLoggerInterface.h"
#include "crossdllinterface\EngineInterface.h"

#include "datastandart/SVGenMapDataStandartInterface.h"
#include "datastandart/PathStorageDataStandartInterface.h"
#include "datastandart/OptimizedPathStorageDataStandartInterface.h"
#include "datastandart/ChartObjectDataStandartInterface.h"

#include "navdisp\OrderBase.h"
#include "colreg\SimulationStructs.h"

#include "SimulationModel/LayerChartObjectImpl.h"
#include "SimulationModel/LayerUnitImpl.h"

#include "simulation\LayerUnitInterface.h"
#include "simulation\LayerChartObjectInterface.h"

namespace SV::surface_simulation
{
   class SimulatorBase
      : public iSimulator
      , public iSimulationState
      , public Central
      , public Servicable
   {
      struct scenario_data
      {
         //settings::unit_source_data unit_data;
         std::vector<LayerChartObjectImpl> chart_objects;
         //layer_provider::layer_chart_object_vct chart_objects;
         std::vector<LayerRoverImpl> land_unit_objects;
         std::vector<LayerDroneImpl> air_unit_objects;
         //layer_provider::layer_unit_object_vct land_unit_objects;
         //layer_provider::layer_unit_object_vct air_unit_objects;
         size_t step_count;
         size_t current_step;

         pathfinder::UnsignedMatrix land_explication;
         pathfinder::UnsignedMatrix air_explication;
         std::vector<pathfinder::UnsignedMatrix> coverage_history;
      };
   public:
      SimulatorBase(central_pack*, iPropertyInterface*, navigation_dispatcher::iComService*);

      virtual ~SimulatorBase() {}

      void Release() override final { delete this; }

      // iSimulator impl
      bool LoadProcessedStep(PROCESS_STEP_TYPE) override final;
      
      //virtual const std::shared_ptr<settings::application_settings>& GetAppSettings() const override final { return GetSettings(); }

      inline void SetSimulationType(const SIMULATION_PLAYER_TYPE type) { m_simulationType = type; }
      void SetAppSettings(const settings::application_settings& settings) override final { m_settings = settings; }
      void SetStepSettings(PROCESS_STEP_TYPE type, const settings::application_settings& settings) { m_stepSettings[type] = settings; }
      const settings::application_settings& GetAppSettings() const override final { return m_settings; }
      const settings::application_settings& GetStepSettings(PROCESS_STEP_TYPE type) const override final { return m_stepSettings.at(type); }

      virtual SCENARIO_STATUS GetSimulatorScenarioState() const override final { return m_scenarioStatus; }
      virtual SIMULATION_STATUS GetSimulatorSimulationState() const override final { return m_simulationStatus; }
      virtual void SetSimulatorScenarioState(SCENARIO_STATUS status) override final { m_scenarioStatus = status; }
      virtual void SetSimulatorSimulationState(SIMULATION_STATUS status) override final { m_simulationStatus = status; }

      // iSimulationState impl
      size_t GetUnitCount(UNIT_TYPE type) const override final;

      const iLayerUnit* GetUnitByIdx(UNIT_TYPE type, size_t idx) const override final;
      const iLayerUnit* GetUnitById(id_type id) const override final;

      size_t GetChartObjectCount() const override final { return m_data.chart_objects.size(); }

      const iLayerChartObject* GetChartObjectByIdx(size_t idx) const override final;
      const iLayerChartObject* GetChartObjectById(chart_object_id id) const override final;

      const pathfinder::UnsignedMatrix& GetLandUnitExplication() const override final { return m_data.land_explication; }
      const pathfinder::UnsignedMatrix& GetAirUnitExplication() const override final { return m_data.air_explication; }
      const std::vector<pathfinder::UnsignedMatrix>& GetCoverageHistory() const override final { return m_data.coverage_history; }
   protected:
      bool checkOpenScenario();
      bool loadProcessedMap();
      bool loadProcessedMapObjects();
      bool loadProcessedPaths();
      bool loadProcessedOptPaths();

      void processRecountRouteVisualizeMeta(layer_provider::layer_unit_object_vct&);
      void calcStepCount();
      surface_simulation::iLayerUnit* getUnitByIdx(UNIT_TYPE type, size_t idx);
      //void deserializeStandartAttrs()
   protected:
      scenario_data m_data;

      std::shared_ptr<file_utils::global_path_storage> m_paths;

      settings::application_settings m_settings;
      std::unordered_map<PROCESS_STEP_TYPE, settings::application_settings> m_stepSettings;

      iPropertyInterface* m_prop;
      std::wstring m_orderCacheFolder, m_currentConfig;

      surface_simulation::SIMULATION_PLAYER_TYPE m_simulationType = surface_simulation::SIMULATION_PLAYER_TYPE::SPT_SIZE;
      
      system::ModuleGuard<database::iSVGMDatabaseController, central_pack*> m_databaseController;
      system::ModuleGuard<engine::iEngine, central_pack*> m_engine;
      system::ModuleGuard<logger::iUniversalLogger, central_pack*> m_logger;

      system::ModuleGuard<data_standart::iDataStandart, central_pack*, LPCSTR, navigation_dispatcher::iComService*> m_mapDS;
      system::ModuleGuard<data_standart::iDataStandart, central_pack*, LPCSTR, navigation_dispatcher::iComService*> m_mapObjDS;
      system::ModuleGuard<data_standart::iDataStandart, central_pack*, LPCSTR, navigation_dispatcher::iComService*> m_pathDS;
      system::ModuleGuard<data_standart::iDataStandart, central_pack*, LPCSTR, navigation_dispatcher::iComService*> m_optPathDS;
      SCENARIO_STATUS m_scenarioStatus = SCENARIO_STATUS::SS_NOT_LOADED;
      SIMULATION_STATUS m_simulationStatus = SIMULATION_STATUS::SS_STOP;
      //dbg::iDebugInfo* m_debugInfo = nullptr;
      navigation_dispatcher::iComService* m_service = nullptr;

      std::vector<std::vector<double>> m_coordGrid;
      chart_grid_meta m_gridMeta = {};
   };
}