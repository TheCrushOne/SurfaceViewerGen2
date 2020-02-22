#include "stdafx.h"
#include "XMLDatabaseController.h"

using namespace database;

XMLDatabaseController::XMLDatabaseController()
{}

void XMLDatabaseController::SaveExternalData(const file_utils::file_storage_base& dst, const double** coordGrid, const chart_meta& meta)
{
   //auto& destFs = reinterpret_cast<file_utils::xml_database_file_storage&>(const_cast<file_utils::file_storage_base&>(dst));
}

database::iSVGMDatabaseController* CreateXMLDatabaseController()
{
   return new database::XMLDatabaseController();
}