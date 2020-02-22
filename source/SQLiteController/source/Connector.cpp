#include "stdafx.h"
#include "Connector.h"

//#pragma comment(lib, "SQLiteWrapper.lib")

using namespace database;

const char* SQL =
   "CREATE TABLE IF NOT EXISTS foo(a,b,c); \
   INSERT INTO FOO VALUES(1,2,3); \
   INSERT INTO FOO SELECT * FROM FOO;"
;

Connector::Connector()
{
   
}

void Connector::Connect(const char* baseFileName)
{
   /*// открываем соединение
   if (sqlite3_open(baseFileName, &m_db))
   {
      fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(m_db));
      return;
   }
   checkRecreateDatabaseStructure();
   // выполняем SQL
   if (sqlite3_exec(m_db, SQL, 0, 0, &m_err))
   {
      fprintf(stderr, "Ошибка SQL: %sn", m_err);
      sqlite3_free(m_err);
   }
   // закрываем соединение
   sqlite3_close(m_db);*/
}

void Connector::checkRecreateDatabaseStructure()
{

}