#pragma once

#include "common/communicator.h"
#include "SimulatorBase.h"
#include "common/file_storage.h"
#include "crossdllinterface/SimulationInterface.h"
#include <array>

namespace SV
{
   using iSimulatorPtr = std::unique_ptr<surface_simulation::SimulatorBase, std::function<void(surface_simulation::SimulatorBase*)>>;

   class SimulatorManager : public surface_simulation::iSimulatorManager, public Central
   {
   public:
      SimulatorManager(central_pack* pack) : Central(pack) {}

      void Release() override final { delete this; }

      void SetPropertyInterface(iPropertyInterface* prop) override final;

      surface_simulation::iSimulator* Get(navigation_dispatcher::iComService* service) override final;

   private:
      surface_simulation::SIMULATION_PLAYER_TYPE getTypeFromExt(const char* ext) const;
      surface_simulation::SIMULATION_PLAYER_TYPE correctXmlTypeByContent(const char* filename) const;

      iSimulatorPtr createSimulationPlayer(surface_simulation::SIMULATION_PLAYER_TYPE type, navigation_dispatcher::iComService* service);
   private:
      iPropertyInterface* m_prop = nullptr;
      file_utils::global_path_storage m_paths;

      std::array<iSimulatorPtr, static_cast<size_t>(surface_simulation::SIMULATION_PLAYER_TYPE::SPT_SIZE)> m_sims;
   };
}