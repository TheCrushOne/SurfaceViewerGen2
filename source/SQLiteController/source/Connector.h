#pragma once

#include "database/sqlite3.h"
#include <functional>

namespace database
{
   class Connector 
   {
   public:
      Connector();

      void Connect(const char* baseFileName);
      void Disconnect();
      void SQLRequest(const char* sql, int(*callback)(void*, int, char**, char**));
   protected:
   private:
      sqlite3* m_db = nullptr;
      char* m_err = nullptr;
   };
}