#pragma once

#include "common/communicator.h"
#include "SimulatorBase.h"
#include "common/file_storage.h"
#include "crossdllinterface/SimulationInterface.h"
#include <array>

using iSimulatorPtr = std::unique_ptr<ColregSimulation::SimulatorBase, std::function<void(ColregSimulation::SimulatorBase*)>>;

class SimulatorManager : public ColregSimulation::iSimulatorManager, public Communicable
{
public:
   SimulatorManager()
      : Communicable(nullptr)
   {}
   void Release() final
   {
      delete this;
   }

   void Init(ICommunicator* pCommunicator, iPropertyInterface* prop) final;

   ColregSimulation::iSimulator* Get(const file_utils::sqlite_database_file_storage& paths) final;

private:
   ColregSimulation::SIMULATION_PLAYER_TYPE getTypeFromExt(const char* ext) const;
   ColregSimulation::SIMULATION_PLAYER_TYPE correctXmlTypeByContent(const char* filename) const;

   iSimulatorPtr createSimulationPlayer(ColregSimulation::SIMULATION_PLAYER_TYPE type);

private:
   iPropertyInterface* m_prop = nullptr;
   file_utils::sqlite_database_file_storage m_paths;

   std::array<iSimulatorPtr, static_cast<size_t>(ColregSimulation::SIMULATION_PLAYER_TYPE::SPT_SIZE)> m_sims;
};