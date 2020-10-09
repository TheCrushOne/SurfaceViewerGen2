#pragma once
#include "crossdllinterface\ChecksumServiceInterface.h"
#include "md5.h"

namespace checksum
{
   class ChecksumService
      : public iChecksumService
      , public Central
   {
   public:
      ChecksumService(central_pack* pack)
         : Central(pack)
      {}
      const char* CalcHash(const char* filename) override final;
      void Release() override final { delete this; }
   private:
      MD5 m_md5hasher;
   };
}