#include "stdafx.h"
#include "Connector.h"

#pragma comment(lib, "SQLiteWrapper.lib")

using namespace database;

Connector::Connector()
{
   
}

void Connector::Connect(const char* baseFileName)
{
   // открываем соединение
   if (sqlite3_open(baseFileName, &m_db))
   {
      fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(m_db));
      return;
   }
}

void Connector::Disconnect()
{
   sqlite3_close(m_db);
}

void Connector::SQLRequest(const char* sql, int(*callback)(void*, int, char**, char**))
{
   if (sqlite3_exec(m_db, sql, callback, 0, &m_err))
   {
      fprintf(stderr, "Ошибка SQL: %sn", m_err);
      sqlite3_free(m_err);
   }
}