#include "stdafx.h"
#include "XMLDatabaseController.h"

using namespace database;

XMLDatabaseController::XMLDatabaseController()
{}

void XMLDatabaseController::Init(const file_utils::file_storage_base& dst)
{}

void XMLDatabaseController::SaveScenarioData(const settings::application_settings& settings, const double** coordGrid)
{
   //auto& destFs = reinterpret_cast<file_utils::xml_database_file_storage&>(const_cast<file_utils::file_storage_base&>(dst));
}

void XMLDatabaseController::SaveAppSettings(const settings::application_settings& settings)
{

}

database::iSVGMDatabaseController* CreateXMLDatabaseController()
{
   return new database::XMLDatabaseController();
}