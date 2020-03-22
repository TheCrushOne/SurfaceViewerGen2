#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/central_class.h"

namespace file_storage
{
   struct iFileStorageManager : colreg::iReleasable
   {
      virtual void Init(ICommunicator*) = 0;
      virtual void PrepareStorage(const wchar_t*) = 0;
   };
}

#ifdef FSM_DLL
#define FSMEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define FSMEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif


extern "C" FSMEXPRTIMPRT file_storage::iFileStorageManager * CreateFileStorageManager();