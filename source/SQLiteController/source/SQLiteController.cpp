#include "stdafx.h"
#include "SQLiteController.h"

using namespace database;

SQLiteController::SQLiteController()
{}

void SQLiteController::SaveExternalData(const file_utils::file_storage_base& dst, const double** coordGrid, const chart_meta& meta)
{
   auto& destFs = reinterpret_cast<file_utils::xml_database_file_storage&>(const_cast<file_utils::file_storage_base&>(dst));
}

database::iSVGMDatabaseController* CreateSQLiteDatabaseController()
{
   return new database::SQLiteController();
}