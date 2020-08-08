#pragma once

#include "colreg/ColregSimulation.h"
#include "common/central_class.h"

namespace ColregSimulation
{
   struct iSimulatorManager : colreg::iReleasable
   {
      //! ���������� ��������� �����������
      virtual void SetPropertyInterface(iPropertyInterface*) = 0;

      //! ��������� ���������� � ����������� �� ���������� �������� � �������� ��������
      virtual iSimulator* Get() = 0;
   };
}


#ifdef SIMULATION_DLL
#define SIMEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SIMEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SIMEXPRTIMPRT ColregSimulation::iSimulatorManager * __cdecl CreateSimulationManager(central_pack*);