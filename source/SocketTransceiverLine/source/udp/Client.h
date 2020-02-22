#pragma once

#include <functional>

class Client
{
public:
   Client(const char* serverAddr, const char* clientPort, std::function<void(const char*)> traceCallback);

   void Init();
   void Send(const char* data);
private:
   SOCKET m_connectSocket = INVALID_SOCKET;
   std::function<void(const char*)> m_traceCallback;
   LPCSTR m_sAddr;
   LPCSTR m_cPort;
};