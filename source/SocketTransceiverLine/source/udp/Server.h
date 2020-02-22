#pragma once

#include <functional>

class Server
{
public:
   Server(const char* serverAddr, const char* serverPort, std::function<void(const char*)> traceCallback, std::function<void(const char*)> dataCallback);

   void Init();
private:
   std::function<void(const char*)> m_traceCallback;
   std::function<void(const char*)> m_dataCallback;
   LPCSTR m_sAddr;
   LPCSTR m_sPort;
};