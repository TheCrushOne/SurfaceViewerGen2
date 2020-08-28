#include "stdafx.h"
#include "ChecksumService.h"

using namespace checksum;

const char* ChecksumService::CalcHash(const char* filename)
{
   return m_md5hasher.digestFile(const_cast<char*>(filename));
}
