#pragma once

#include "crossdllinterface\TransceiverInterface.h"
#include <functional>

class Server
{
public:
   Server(const transceiver::transceiver_info& info);

   void Init();
private:
   const transceiver::transceiver_info& m_info;

   //std::function<void(const char*)> m_traceCallback;
   //std::function<void(const char*)> m_dataCallback;
   //LPCSTR m_sAddr;
   //LPCSTR m_sPort;
};