#pragma once
#include "stdafx.h"
#include "colreg/Communicator.h"
#include "user_interface.h"

#include "scenario/ScenarioManager.h"

class CommunicatorWnd : public ICommunicator
{
public:
   bool stdCallConv Message(ICommunicator::MessageType t, const char* msg, ...) override final
   {
      va_list list;
      va_start(list, msg);
      char buffer[512];
      vsprintf(buffer, msg, list);
      user_interface::SetOutputText(static_cast<user_interface::OUTPUT_TYPE>(t), buffer);
      va_end(list);
      return true;
   }
   void stdCallConv SetProgress(unsigned int progress) override final
   {
      user_interface::SetProgress(progress);
   }

   bool stdCallConv GetTerminateFlag() override final
   {
      return true;
   }

   bool stdCallConv UpdateUI() override final
   {
      user_interface::InvalidateView();
      return true;
   }

   bool stdCallConv RaiseError() override final
   {
      user_interface::RaiseError();
      return true;
   }

   bool stdCallConv OnPathFound() override final
   {
      return ScenarioManager::GetInstance().OnScenarioPathFound();
   }
};