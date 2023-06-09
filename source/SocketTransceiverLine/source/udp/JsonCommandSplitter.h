#pragma once

#include "common\transceiver_types.h"
#include <string>

class JsonCommandSplitter abstract
{
public:
   static transceiver::JsonCommand GetCommandToken(const char* jsonDee);
   static std::string PackCommand(transceiver::JsonCommand command, const char* jsonDee);
};