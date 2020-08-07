#include "stdafx.h"
#include "Simulator.h"
#include "gui/WndCommunicator.h"
//#include "TTCG\Common\FileSystem\Path.h"
#include "gui/layers/RenderLayers.h"
#include "crossdllinterface\SimulationInterface.h"

namespace
{
   colreg::ModuleGuard<ColregSimulation::iSimulatorManager, central_pack*> _simMgr;
   std::unique_ptr<CommunicatorWnd> comm(new CommunicatorWnd);
   std::shared_ptr<central_pack> pack = std::make_shared<central_pack>(comm.get()/*, file_utils::global_path_storage{}, settings::application_settings{}*/);  
   ColregSimulation::iSimulator* _simulator = nullptr;

   bool createSimulationManager()
   {
      if (_simMgr.IsValid())
      {
         return true;
      }
      //const fpath simulatorPath = fpath::get_module_folder().append("ColregSimulation.dll");
      bool retval = _simMgr.Create(SVGUtils::CurrentDllPath("SurfaceSimulation").c_str(), "CreateSimulationManager", pack.get());
      if (!_simMgr.IsValid())
      {
         user_interface::RaiseError();
         std::string errMsg = std::string("Can't load '") + "SurfaceSimulation" + "'!";
         user_interface::SetOutputText(user_interface::OT_ERROR, errMsg.c_str());
         return false;
      }
      return retval;
   }
}

namespace simulator
{
   ICommunicator* GetCommunicator()
   {
      return pack->comm.get();
   }

   ColregSimulation::iSimulator* getSimulator()
   {
      return _simulator;
   }

   central_pack* GetPack()
   {
      return pack.get();
   }

   void simulatorStop(bool simulatorReset)
   {
      if (!_simulator)
         return;

      simulatorReset ? _simulator->Reset() : _simulator->Stop();
   }

   // TODO: раскомментить, если понадобится
   /*bool simulatorInit()
   {
      if (!_simulator)
         createSimulationManager();
   }*/

   bool simulatorStart()
   {
      if (!_simulator)
         createSimulationManager();

      simulatorStop();

      _simulator = nullptr;

      _simulator = _simMgr->Get();

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

      _simulator->NextControlPoint();

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
