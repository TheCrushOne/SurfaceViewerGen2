#include "stdafx.h"
#include "SimulatorBase.h"

using namespace ColregSimulation;

bool SimulatorBase::Init(const file_utils::global_path_storage& paths)
{
   m_databaseController.Create(SVGUtils::CurrentDllPath("SQLiteController").c_str(), "CreateSQLiteDatabaseController");
   if (!m_databaseController.IsValid())
   {
      //m_lock = true;
      //MessageString(ICommunicator::MS_Error, "Can't load settings serializer!");
      return false;
   }
   m_databaseController->Init(m_communicator, paths);
   return true;
}

