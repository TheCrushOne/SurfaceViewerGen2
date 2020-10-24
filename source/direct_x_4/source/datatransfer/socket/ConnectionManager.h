#pragma once

#include "colreg\ModuleGuard.h"
#include "crossdllinterface\TransceiverInterface.h"
#include "crossdllinterface\DataShareInterface.h"

class Engine;
class Infoboard;

class ConnectionManager
{
public:
   ConnectionManager(std::shared_ptr<Engine>& engine);
private:
   void createTransceiver();
   void createDataShareProvider();
   void initTransceiver();
   void callback(const char*);
   void parseIncomingCommand(const char*);
private:
   SV::system::ModuleGuard<SV::transceiver::iTransceiver> m_transceiver;
   SV::system::ModuleGuard<SV::data_share::iDataShareProvider> m_shareProvider;
   std::shared_ptr<Infoboard>& m_infoboard;
   std::shared_ptr<Engine>& m_engine;
   SV::transceiver::transceiver_info m_info;
   double** m_rawdata;
};