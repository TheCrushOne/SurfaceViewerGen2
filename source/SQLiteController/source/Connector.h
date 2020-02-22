#pragma once

#include "database/sqlite3.h"

namespace database
{
   class Connector 
   {
   public:
      Connector();

      void Connect(const char* baseFileName);
   protected:
      void checkRecreateDatabaseStructure();
   private:
      sqlite3* m_db = nullptr;
      char* m_err = nullptr;
   };
}