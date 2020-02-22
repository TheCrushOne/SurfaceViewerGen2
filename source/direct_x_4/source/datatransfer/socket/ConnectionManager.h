#pragma once

#include "colreg\ModuleGuard.h"
#include "crossdllinterface\TransceiverInterface.h"
#include "crossdllinterface\DataShareInterface.h"

class Infoboard;

class ConnectionManager
{
public:
   ConnectionManager(std::shared_ptr<Infoboard>& infoboard);
private:
   void createTransceiver();
   void createDataShareProvider();
   void initTransceiver();
   void callback(const char*);
   void parseIncomingCommand(const char*);
private:
   colreg::ModuleGuard<iTransceiver> m_transceiver;
   colreg::ModuleGuard<data_share::iDataShareProvider> m_shareProvider;
   std::shared_ptr<Infoboard>& m_infoboard;
   double** m_rawdata;
};