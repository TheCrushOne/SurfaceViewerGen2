#pragma once

#include "colreg/ColregSimulation.h"

namespace ColregSimulation
{
   struct iSimulatorManager : colreg::iReleasable
   {
      //! ���������� ��������� �����������
      virtual void Init(ICommunicator* pCommunicator, iPropertyInterface* prop) = 0;

      //! ��������� ���������� � ����������� �� ���������� �������� � �������� ��������
      virtual iSimulator* Get(const file_utils::sqlite_database_file_storage& paths) = 0;
   };
}


#ifdef SIMULATION_DLL
#define SIMEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define SIMEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" SIMEXPRTIMPRT ColregSimulation::iSimulatorManager * __cdecl CreateSimulationManager();