#include "stdafx.h"
#include "XMLDatabaseController.h"

using namespace database;

XMLDatabaseController::XMLDatabaseController()
{}

void XMLDatabaseController::Init(ICommunicator* comm)
{}

void XMLDatabaseController::SaveScenarioData(const settings::unit_source_data& unitData, const std::vector<std::vector<double>>& coordGrid)
{
   //auto& destFs = reinterpret_cast<file_utils::xml_database_file_storage&>(const_cast<file_utils::file_storage_base&>(dst));
}

void XMLDatabaseController::SaveAppSettings()
{
}

void XMLDatabaseController::LoadScenarioData(settings::unit_source_data& unitData, std::vector<std::vector<double>>& coordGrid)
{
}

void XMLDatabaseController::LoadAppSettings()
{
}

database::iSVGMDatabaseController* CreateXMLDatabaseController()
{
   return new database::XMLDatabaseController();
}