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

   m_engine.Create(SVGUtils::CurrentDllPath("Engine").c_str(), "CreateEngine");
   if (!m_engine.IsValid())
   {
      Message(ICommunicator::MS_Error, "Can't load 'Engine'");
      return false;
   }
   m_engine->Init(GetPack());

   m_generator.Create(SVGUtils::CurrentDllPath("ChartObjectGenerator").c_str(), "CreateGenerator");
   if (!m_generator.IsValid())
   {
      Message(ICommunicator::MS_Error, "Can't load 'ChartObjectGenerator'");
      return false;
   }

   m_logger.Create(SVGUtils::CurrentDllPath("UniversalLogger").c_str(), "CreateUniversalLogger");
   if (!m_logger.IsValid())
   {
      Message(ICommunicator::MS_Error, "Can't load 'UniversalLogger'");
      return false;
   }
   m_logger->Init(GetPack());

   return true;
}

