#pragma once

#include "database/sqlite3.h"
#include <functional>
#include "common/communicator.h"
#include "common/central_class.h"

namespace SV::database
{
   class Connector : public Central
   {
   public:
      Connector(central_pack* pack)
         : Central(pack)
      {}
      void Connect(const char* baseFileName);
      void Disconnect();
      void SQLNoResRequest(const char* sql);
      void SQLResRequest(const char* sql, std::function<void(sqlite3_stmt*)> callback);
   protected:
   private:
      sqlite3* m_db = nullptr;
      char* m_err = nullptr;
   };
}