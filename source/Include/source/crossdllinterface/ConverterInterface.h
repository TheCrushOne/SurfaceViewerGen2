#pragma once

namespace converter
{
   struct iConverter
   {
      virtual bool Convert(const char* srcPath, const char* dstPath) = 0;
   };
}