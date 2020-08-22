#include "stdafx.h"
#include "XMLDatabaseController.h"

using namespace database;

XMLDatabaseController::XMLDatabaseController(central_pack* pack)
   : Central(pack)
{}

void XMLDatabaseController::Connect(const char*)
{
}

void XMLDatabaseController::SaveScenarioData(const settings::unit_source_data& unitData, const std::vector<std::vector<double>>& coordGrid)
{
   //auto& destFs = reinterpret_cast<file_utils::xml_database_file_storage&>(const_cast<file_utils::file_storage_base&>(dst));
}

void XMLDatabaseController::SaveAppSettings()
{
}

void XMLDatabaseController::SaveDataStandartHashJunction(data_hash::hash_junction&)
{
}

void XMLDatabaseController::LoadScenarioData(settings::unit_source_data& unitData, std::vector<std::vector<double>>& coordGrid)
{
}

void XMLDatabaseController::LoadAppSettings()
{
}

bool XMLDatabaseController::CheckDataStandartHashJunction(data_hash::hash_junction&)
{
   return true;
}

database::iSVGMDatabaseController* CreateXMLDatabaseController(central_pack* pack)
{
   return new database::XMLDatabaseController(pack);
}