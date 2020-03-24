#include "stdafx.h"
#include "SimulatorBase.h"

using namespace ColregSimulation;

bool SimulatorBase::Init(central_pack* pack)
{
   Central::Init(pack);

   m_databaseController.Create(SVGUtils::CurrentDllPath("SQLiteController").c_str(), "CreateSQLiteDatabaseController");
   if (!m_databaseController.IsValid())
   {
      //m_lock = true;
      Message(ICommunicator::MS_Error, "Can't load 'SQLiteController'!");
      return false;
   }
   m_databaseController->Init(pack);
   return true;
}

