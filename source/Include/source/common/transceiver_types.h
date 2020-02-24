#pragma once

#include <string>
#include <unordered_map>
#include <functional>

namespace transceiver
{
   enum class JsonCommand : unsigned short
   {
      JC_NEWSURFACE,
   };

   struct transceiver_info
   {
      std::string server_addr;
      std::string server_port;
      std::string client_port;
      std::function<void(const char*)> trace_callback;
      std::unordered_map<JsonCommand, std::function<void(const char*)>> data_callback_map;
   };
}

namespace
{
   static std::unordered_map<transceiver::JsonCommand, std::string> tokenMap = {
      { transceiver::JsonCommand::JC_NEWSURFACE, "JC_NEWSURFACE" }
   };
}

namespace transceiver
{
   inline std::string GetTokenByCommand(JsonCommand command) { return (tokenMap.find(command) != tokenMap.end() ? tokenMap.at(command) : "" ); }
   inline JsonCommand GetCommandByToken(std::string& token)
   {
      for (auto& elem : tokenMap)
      {
         if (elem.second.compare(token) == 0)
            return elem.first;
      }
      // TODO: check!!!
      return JsonCommand::JC_NEWSURFACE;
   }
}