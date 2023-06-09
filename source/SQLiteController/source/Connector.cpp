#include "stdafx.h"
#include "Connector.h"

#pragma comment(lib, "SQLiteWrapper.lib")

using namespace database;

void Connector::Connect(const char* baseFileName)
{
   // ��������� ����������
   if (sqlite3_open(baseFileName, &m_db))
   {
      Message(ICommunicator::MS_Error, "Error opening DB: %s\n", sqlite3_errmsg(m_db));
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
      Message(ICommunicator::MS_Error, "SQL Error: %s", m_err);// (stderr, "������ SQL: %sn", m_err);
      sqlite3_free(m_err);
   }
}

void Connector::SQLResRequest(const char* sql, std::function<void(sqlite3_stmt* )> callback)
{
   sqlite3_stmt* stmt;
   if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, NULL) != SQLITE_OK)
   {
      Message(ICommunicator::MS_Error, "Compiling Error: %s", sqlite3_errmsg(m_db));// (stderr, "������ SQL: %sn", m_err);
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
      Message(ICommunicator::MS_Error, "Performing Error: %s\n", sqlite3_errmsg(m_db));
   }
}