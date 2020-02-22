#pragma once

#include "crossdllinterface\TransceiverInterface.h"
#include "Client.h"
#include "Server.h"

class TransceiverImpl : public iTransceiver
{
public:
   TransceiverImpl();

   void Init(const char* serverAddr, const char* serverPort, const char* clientPort, std::function<void(const char*)> traceCallback, std::function<void(const char*)> dataCallback);
   void Send(const char* message);
   void Release() override { delete this; }
protected:
   void initServer();
   void initClient();
private:
   std::unique_ptr<Client> m_client;
   std::unique_ptr<Server> m_server;
};