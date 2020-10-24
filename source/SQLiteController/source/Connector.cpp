#include "stdafx.h"
#include "Connector.h"
#include <filesystem>

#pragma comment(lib, "SQLiteWrapper.lib")

using namespace SV;
using namespace SV::database;

void Connector::Connect(const char* baseFileName)
{
   // NOTE: каталога может не быть
   std::filesystem::path path(baseFileName);
   auto dir = path.parent_path();
   std::filesystem::create_directories(dir);
   // открываем соединение
   if (sqlite3_open(baseFileName, &m_db))
   {
      Message(ICommunicator::MessageType::MT_ERROR, "Error opening DB: %s\n", sqlite3_errmsg(m_db));
      return;
   }
}

void Connector::Disconnect()
{
   sqlite3_close(m_db);
}

void Connector::SQLNoResRequest(const char* sql)
{
   if (sqlite3_exec(m_db, sql, nullptr, 0, &m_err))
   {
      Message(ICommunicator::MessageType::MT_ERROR, "SQL Error: %s", m_err);// (stderr, "Ошибка SQL: %sn", m_err);
      sqlite3_free(m_err);
   }
}

void Connector::SQLResRequest(const char* sql, std::function<void(sqlite3_stmt* )> callback)
{
   sqlite3_stmt* stmt;
   if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, NULL) != SQLITE_OK)
   {
      Message(ICommunicator::MessageType::MT_ERROR, "Compiling Error: %s", sqlite3_errmsg(m_db));// (stderr, "Ошибка SQL: %sn", m_err);
      sqlite3_free(m_err);
      sqlite3_finalize(stmt);
   }
   int ret_code = 0;
   while ((ret_code = sqlite3_step(stmt)) == SQLITE_ROW)
   {
      callback(stmt);//printf("TEST: ID = %d\n", sqlite3_column_int(stmt, 0));
   }
   if (ret_code != SQLITE_DONE)
   {
      //this error handling could be done better, but it works
      Message(ICommunicator::MessageType::MT_ERROR, "Performing Error: %s\n", sqlite3_errmsg(m_db));
   }
}