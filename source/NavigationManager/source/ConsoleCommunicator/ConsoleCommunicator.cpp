#include "stdafx.h"
#include "ConsoleCommunicator.h"
#include <iostream>

using namespace SV;

bool stdCallConv ConsoleCommunicator::Message(ICommunicator::MessageType t, const char* msg, ...)
{
   va_list list;
   va_start(list, msg);
   char buffer[512];
   vsprintf(buffer, msg, list);
   std::cout << typeMap.at(t) << buffer << std::endl;
   //user_interface::SetOutputText(static_cast<user_interface::OUTPUT_TYPE>(t), buffer);
   va_end(list);
   return true;
}

void stdCallConv ConsoleCommunicator::SetProgress(unsigned int progress)
{
   //user_interface::SetProgress(progress);
}