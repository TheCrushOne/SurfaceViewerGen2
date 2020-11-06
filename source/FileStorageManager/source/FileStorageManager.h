#pragma once

#include "crossdllinterface/FileStorageInterface.h"
#include "common/central_class.h"

#include "json/json_wrapper.h"

namespace SV::file_storage
{
   struct tag
   {
      static constexpr char log_rel_path[] = "log_rel_path";
   };
   class FileStorageManager
      : public iFileStorageManager
      , public Central
   {
   public:
      FileStorageManager(central_pack* pack)
         : Central(pack)
      {}
      void Release() override final { delete this; }
      void PrepareStorage(const char* metaFileName);
   };
}