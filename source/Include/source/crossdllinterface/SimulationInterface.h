#pragma once

#include "colreg/SimulatorInterface.h"
#include "common/central_class.h"
#include "navdisp\ComService.h"

namespace SV::surface_simulation
{
   struct iSimulatorManager : iReleasable
   {
      //! ���������� ��������� �����������
      virtual void SetPropertyInterface(iPropertyInterface*) = 0;

      //! ��������� ���������� � ����������� �� ���������� �������� � �������� ��������
      virtual iSimulator* Get(navigation_dispatcher::iComService*) = 0;
   };
}


#ifdef SURFACESIMULATION_EXPORTS
#define SIMEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SIMEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SIMEXPRTIMPRT SV::surface_simulation::iSimulatorManager * __cdecl CreateSimulationManager(SV::central_pack*);