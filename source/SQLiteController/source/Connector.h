#pragma once

#include "database/sqlite3.h"
#include <functional>
#include "common/communicator.h"
#include "common/central_class.h"

namespace database
{
   class Connector : public Central
   {
   public:
      void Init(central_pack* pack) { Central::Init(pack); }

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