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
   colreg::ModuleGuard<transceiver::iTransceiver> m_transceiver;
   colreg::ModuleGuard<data_share::iDataShareProvider> m_shareProvider;
   std::shared_ptr<Infoboard>& m_infoboard;
   std::shared_ptr<Engine>& m_engine;
   transceiver::transceiver_info m_info;
   double** m_rawdata;
};