#pragma once

#include "crossdllinterface/FileStorageInterface.h"
#include "common/central_class.h"

namespace file_storage
{
   class FileStorageManager : public iFileStorageManager, public Central
   {
   public:
      FileStorageManager(central_pack* pack)
         : Central(pack)
      {}
      void Release() override final { delete this; }
      void PrepareStorage(const wchar_t* metaFileName);
   };
}