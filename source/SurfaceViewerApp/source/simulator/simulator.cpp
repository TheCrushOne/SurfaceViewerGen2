#include "stdafx.h"
#include "Simulator.h"
#include "gui/WndCommunicator.h"
//#include "TTCG\Common\FileSystem\Path.h"
#include "gui/layers/RenderLayers.h"
#include "crossdllinterface\SimulationInterface.h"

namespace
{
   colreg::ModuleGuard<ColregSimulation::iSimulatorManager> _simMgr;
   std::unique_ptr<CommunicatorWnd> comm(new CommunicatorWnd);
   ColregSimulation::iSimulator* _simulator = nullptr;

   bool createSimulationManager()
   {
      if (_simMgr.IsValid())
      {
         return true;
      }
      //const fpath simulatorPath = fpath::get_module_folder().append("ColregSimulation.dll");
      auto simmgr = _simMgr.Create(SVGUtils::CurrentDllPath("SurfaceSimulation").c_str(), "CreateSimulationManager");
      _simMgr->Init(comm.get(), LayersContainer::GetLayerProperties());
      return simmgr;
   }
}

namespace simulator
{
   ICommunicator* GetCommunicator()
   {
      return comm.get();
   }

   ColregSimulation::iSimulator* getSimulator()
   {
      return _simulator;
   }

   void simulatorStop(bool simulatorReset)
   {
      if (!_simulator)
         return;

      simulatorReset ? _simulator->Reset() : _simulator->Stop();
   }

   bool simulatorStart(const file_utils::sqlite_database_file_storage& fs)
   {
      if (!_simulator)
         createSimulationManager();

      simulatorStop();

      _simulator = nullptr;

      _simulator = _simMgr->Get(fs);

      ATLASSERT(_simulator);

      if (_simulator == nullptr)
      {
         AfxMessageBox(L"Error! Simulator haven`t been started!");
         return false;
      }
      //_simulator->GetDebugInfo();
      _simulator->Start();

      return true;
   }

   bool simulatorStep()
   {
      if (_simulator == nullptr)
         return false;

      //_simulator->NextControlPoint();

      //auto est = _simulator->GetState().GetEstimation();

      //if (est && est->GetResult() && (est->GetResult()->GetDangersCount() > 0))
      //{
      //   const auto ownship_id = _simulator->GetSimulationSettings().ownship_id;
      //   if (ownship_id != colreg::INVALID_ID)
      //   {
      //      for (const auto& cluster : *est->GetResult()->GetClusters())
      //      {
      //         for (const auto& danger : *cluster->GetDangers())
      //         {
      //            if (danger.ship_id == ownship_id)
      //               return false;
      //         }
      //      }
      //   }
      //   else
      //      return false;
      //}
      return true;
   }

   colreg::geo_point getCenter()
   {
      colreg::geo_point center{ 0., 0. };
      if (!_simulator)
         return center;

      /*const auto& simulationState = _simulator->GetState();

      const auto count = simulationState.GetShipCount();
      if (count == 0)
         return center;
      for (size_t iShip = 0; iShip < count; ++iShip)
      {
         center.lat += simulationState.GetShip(iShip).GetPos().point.pos.lat;
         center.lon += simulationState.GetShip(iShip).GetPos().point.pos.lon;
      }

      center.lat /= count;
      center.lon /= count;*/

      return center;
   }
}
