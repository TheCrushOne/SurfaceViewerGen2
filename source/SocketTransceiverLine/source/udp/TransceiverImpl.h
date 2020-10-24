#pragma once

#include "crossdllinterface\TransceiverInterface.h"
#include "Client.h"
#include "Server.h"
#include "common/central_class.h"

class TransceiverImpl : public SV::transceiver::iTransceiver, public SV::Central
{
public:
   TransceiverImpl(SV::central_pack* pack);

   void Init(const SV::transceiver::transceiver_info& info) override final;
   void Send(SV::transceiver::JsonCommand token, const char* jsonDee) override final;
   void Release() override { delete this; }
protected:
   void initServer();
   void initClient();
private:
   std::unique_ptr<SV::Client> m_client;
   std::unique_ptr<SV::Server> m_server;
};