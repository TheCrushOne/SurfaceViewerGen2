#pragma once
#include "stdafx.h"
#include "colreg/Communicator.h"
#include "user_interface.h"

class CommunicatorWnd : public ICommunicator
{
public:
   bool stdCallConv Message(MessageType t, const char* msg, ...)
   {
      va_list list;
      va_start(list, msg);
      char buffer[512];
      vsprintf(buffer, msg, list);
      user_interface::SetOutputText(static_cast<user_interface::OUTPUT_TYPE>(t), buffer);
      va_end(list);
      return true;
   }
   void stdCallConv SetProgress(unsigned int progress)
   {

   }

   bool stdCallConv GetTerminateFlag()
   {
      return true;
   }

   bool stdCallConv UpdateUI()
   {
      return true;
   }
};