#pragma once

#include "common/communicator.h"
#include "colreg/ColregSimulation.h"
#include "colreg/ModuleGuard.h"
#include "crossdllinterface/SVGMDatabaseInterface.h"
#include "common/simulation_structs.h"

namespace ColregSimulation
{
   class SimulatorBase : public Communicable, public iSimulator
   {
   public:
      SimulatorBase(ICommunicator* pCommunicator, iPropertyInterface* prop)
         : m_prop(prop)
         , Communicable(pCommunicator)
      {}

      virtual ~SimulatorBase() {}

      void Release() override final { delete this; }

      virtual bool Init(const std::shared_ptr<file_utils::global_path_storage> paths);

      inline void SetSimulationType(const ColregSimulation::SIMULATION_PLAYER_TYPE type) { m_simulationType = type; }
   protected:
      std::shared_ptr<file_utils::global_path_storage> m_paths;
      iPropertyInterface* m_prop;
      ColregSimulation::SIMULATION_PLAYER_TYPE m_simulationType = ColregSimulation::SIMULATION_PLAYER_TYPE::SPT_SIZE;
      settings::application_settings m_settings;
      scenario_data m_data;
      colreg::ModuleGuard<database::iSVGMDatabaseController> m_databaseController;
   };
}