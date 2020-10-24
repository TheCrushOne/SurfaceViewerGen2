#include "stdafx.h"
#include "ChecksumService.h"

using namespace SV;
using namespace SV::checksum;

const char* ChecksumService::CalcHash(const char* filename)
{
   return m_md5hasher.digestFile(const_cast<char*>(filename));
}

checksum::iChecksumService* CreateChecksumService(central_pack* pack)
{
   return new ChecksumService(pack);
}
