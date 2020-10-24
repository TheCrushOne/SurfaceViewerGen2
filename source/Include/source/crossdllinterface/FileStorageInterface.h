#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/central_class.h"

namespace SV::file_storage
{
   struct iFileStorageManager : iReleasable
   {
      virtual void PrepareStorage(const wchar_t*) = 0;
   };
}

#ifdef FILESTORAGEMANAGER_EXPORTS
#define FSMEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define FSMEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" FSMEXPRTIMPRT SV::file_storage::iFileStorageManager * CreateFileStorageManager(SV::central_pack*);