#pragma once

#include <unordered_map>

namespace database
{
   enum class SQLRequestType : unsigned short
   {
      SRT_CREATE_TABLE = 0,
   };

   const static std::unordered_map<SQLRequestType, const char*> requestMap = {
      { SRT_CREATE_TABLE, "CREATE TABLE IF NOT EXISTS metadata(,,);" \
                          "" }
   };
}