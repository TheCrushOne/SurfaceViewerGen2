#pragma once

#include "crossdllinterface\TransceiverInterface.h"
#include "Client.h"
#include "Server.h"
#include "common/central_class.h"

class TransceiverImpl : public transceiver::iTransceiver, public Central
{
public:
   TransceiverImpl(central_pack* pack);

   void Init(const transceiver::transceiver_info& info) override final;
   void Send(transceiver::JsonCommand token, const char* jsonDee) override final;
   void Release() override { delete this; }
protected:
   void initServer();
   void initClient();
private:
   std::unique_ptr<Client> m_client;
   std::unique_ptr<Server> m_server;
};