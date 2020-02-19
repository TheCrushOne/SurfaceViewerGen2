#include "stdafx.h"
#include "Simulator.h"
//#include "WndCommunicator.h"
//#include "TTCG\Common\FileSystem\Path.h"
#include "gui/layers/RenderLayers.h"
#include "crossdllinterface\SimulatorManagerInterface.h"

namespace
{
   colreg::ModuleGuard<surface_simulation::iSimulatorManager> _simMgr;

   bool createSimulationManager()
   {
      if (_simMgr.IsValid())
      {
         return true;
      }
      //const fpath simulatorPath = fpath::get_module_folder().append("ColregSimulation.dll");
      auto simmgr = _simMgr.Create(SVGUtils::CurrentDllPath("SettingsHandler").c_str(), "CreateSimulationManager");
      _simMgr->Init(/*comm.get(), LayersContainer::GetLayerProperties()*/);
      return simmgr;
   }
}

namespace simulator
{
   surface_simulation::iSimulator* _simulator = nullptr;

   surface_simulation::iSimulator* getSimulator()
   {
      return _simulator;
   }
   void simulatorStop(bool simulatorReset)
   {
      if (!_simulator)
         return;

      //simulatorReset ? _simulator->Reset() : _simulator->Stop();
   }

   bool simulatorStart(const char* fileName)
   {
      if (!_simulator)
         createSimulationManager();

      simulatorStop();

      _simulator = nullptr;

      _simulator = _simMgr->Get(fileName);

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
}
