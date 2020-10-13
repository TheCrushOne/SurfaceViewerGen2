#pragma once

#include "colreg/ColregSimulation.h"
#include "common/central_class.h"
#include "navdisp\ComService.h"

namespace ColregSimulation
{
   struct iSimulatorManager : colreg::iReleasable
   {
      //! ���������� ��������� �����������
      virtual void SetPropertyInterface(iPropertyInterface*) = 0;

      //! ��������� ���������� � ����������� �� ���������� �������� � �������� ��������
      virtual iSimulator* Get(navigation_dispatcher::iComServicePtr) = 0;
   };
}


#ifdef SURFACESIMULATION_EXPORTS
#define SIMEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SIMEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SIMEXPRTIMPRT ColregSimulation::iSimulatorManager * __cdecl CreateSimulationManager(central_pack*);