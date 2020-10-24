#pragma once

#include "crossdllinterface\TransceiverInterface.h"
#include <functional>

namespace SV
{
   class Client
   {
   public:
      Client(const SV::transceiver::transceiver_info& info);

      void Init();
      void Send(const char* data);
   private:
      const SV::transceiver::transceiver_info& m_info;

      SOCKET m_connectSocket = INVALID_SOCKET;

      //std::function<void(const char*)> m_traceCallback;
      //LPCSTR m_sAddr;
      //LPCSTR m_cPort;
   };
}