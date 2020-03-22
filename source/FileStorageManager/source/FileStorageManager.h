#pragma once

#include "crossdllinterface/FileStorageInterface.h"
#include "common/central_class.h"

namespace file_storage
{
   class FileStorageManager : public iFileStorageManager, public Central
   {
   public:
      FileStorageManager()
         : Central(nullptr)
      {}

      void Init(ICommunicator* comm) override final { Central::Init(comm); }
      void Release() override final { delete this; }
      void PrepareStorage(const wchar_t* metaFileName);
   };
}