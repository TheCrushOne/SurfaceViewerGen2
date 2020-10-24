#pragma once

#include <string>
#include <unordered_map>
#include <functional>

namespace SV::transceiver
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
   static std::unordered_map<SV::transceiver::JsonCommand, std::string> tokenMap = {
      { SV::transceiver::JsonCommand::JC_NEWSURFACE, "JC_NEWSURFACE" }
   };
}

namespace SV::transceiver
{
   inline std::string GetTokenByCommand(SV::transceiver::JsonCommand command) { return (tokenMap.find(command) != tokenMap.end() ? tokenMap.at(command) : "" ); }
   inline SV::transceiver::JsonCommand GetCommandByToken(std::string& token)
   {
      for (auto& elem : tokenMap)
      {
         if (elem.second.compare(token) == 0)
            return elem.first;
      }
      // TODO: check!!!
      return SV::transceiver::JsonCommand::JC_NEWSURFACE;
   }
}