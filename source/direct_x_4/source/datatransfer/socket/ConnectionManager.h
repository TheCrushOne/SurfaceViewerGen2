#pragma once

#include "colreg\ModuleGuard.h"
#include "crossdllinterface\TransceiverInterface.h"

class Infoboard;

class ConnectionManager
{
public:
   ConnectionManager(std::shared_ptr<Infoboard>& infoboard);
private:
   void createTransceiver();
   void initTransceiver();
   void callback(const char*);
private:
   colreg::ModuleGuard<iTransceiver> m_transceiver;
   std::shared_ptr<Infoboard>& m_infoboard;
};