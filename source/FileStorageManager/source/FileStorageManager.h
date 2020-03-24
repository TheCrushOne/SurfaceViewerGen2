#pragma once

#include "crossdllinterface/FileStorageInterface.h"
#include "common/central_class.h"

namespace file_storage
{
   class FileStorageManager : public iFileStorageManager, public Central
   {
   public:
      void Init(std::shared_ptr<central_pack> pack) override final { Central::Init(pack); }
      void Release() override final { delete this; }
      void PrepareStorage(const wchar_t* metaFileName);
   };
}