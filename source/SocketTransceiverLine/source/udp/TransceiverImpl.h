#pragma once

#include "crossdllinterface\TransceiverInterface.h"

class TransceiverImpl : public iTransceiver
{
public:
   TransceiverImpl();

   void Init(const char* serverAddr, const char* serverPort, const char* clientPort, std::function<void(const char*)> callback);
   void Release() override { delete this; }
protected:
   void createServer();
   void createClient();
private:
   std::function<void(const char*)> m_callback;
   LPCSTR m_sAddr;
   LPCSTR m_sPort;
   LPCSTR m_cPort;
};