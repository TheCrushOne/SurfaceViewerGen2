#pragma once

namespace database
{
   struct iSVGMDatabaseController
   {
      virtual void SaveExternalData(const char* dstPath) = 0;
   };
}