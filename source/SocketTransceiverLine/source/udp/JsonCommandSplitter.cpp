#include <nlohmann/json.hpp>
#include "stdafx.h"
#include "JsonCommandSplitter.h"

using namespace nlohmann;

transceiver::JsonCommand JsonCommandSplitter::GetCommandToken(const char* jsonDee)
{
   json j = jsonDee;
   std::string token = j["token"].get<std::string>();
   return transceiver::GetCommandByToken(token);
}

std::string JsonCommandSplitter::PackCommand(transceiver::JsonCommand command, const char* jsonDee)
{
   json j;
   j["token"] = transceiver::GetTokenByCommand(command);
   j["meta"] = std::string(jsonDee);
   return j.dump();
}