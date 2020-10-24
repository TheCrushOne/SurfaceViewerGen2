#include "stdafx.h"
#include "JsonCommandSplitter.h"

using namespace SV;

transceiver::JsonCommand JsonCommandSplitter::GetCommandToken(const char* jsonDee)
{
   //json j = jsonDee;
   std::string token/* = j["token"].get<std::string>()*/;
   return transceiver::GetCommandByToken(token);
}

std::string JsonCommandSplitter::PackCommand(transceiver::JsonCommand command, const char* jsonDee)
{
   //json j;
   //j["token"] = transceiver::GetTokenByCommand(command);
   //j["meta"] = std::string(jsonDee);
   return std::string();// j.dump();
}