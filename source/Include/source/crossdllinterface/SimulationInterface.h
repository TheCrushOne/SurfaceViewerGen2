#pragma once

#include "colreg/ColregSimulation.h"

namespace ColregSimulation
{
   struct iSimulatorManager : colreg::iReleasable
   {
      //! ���������� ��������� �����������
      virtual void Init(ICommunicator*, iPropertyInterface*) = 0;

      //! ��������� ���������� � ����������� �� ���������� �������� � �������� ��������
      virtual iSimulator* Get(const std::shared_ptr<file_utils::global_path_storage>) = 0;
   };
}


#ifdef SIMULATION_DLL
#define SIMEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SIMEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SIMEXPRTIMPRT ColregSimulation::iSimulatorManager * __cdecl CreateSimulationManager();