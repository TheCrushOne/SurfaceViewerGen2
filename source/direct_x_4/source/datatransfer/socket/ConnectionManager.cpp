#include "stdafx.h"
#include "ConnectionManager.h"
#include "common\utils.h"
#include "utils\infoboard.h"
#include "engine\engine.h"

#include <future>
#include <string>
#include <mutex>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

ConnectionManager::ConnectionManager(std::shared_ptr<Engine>& engine)
   : m_infoboard(engine->GetInfoboard())
   , m_engine(engine)
   , m_info({ "127.0.0.1", "27015", "8080", [this](const char* txt) { this->callback(txt); } })
{
   m_info.data_callback_map[transceiver::JsonCommand::JC_NEWSURFACE] = [this](const char* txt) { this->parseIncomingCommand(txt); };
   createTransceiver();
   createDataShareProvider();
}

void ConnectionManager::createTransceiver()
{
   m_transceiver.Create(SVGUtils::CurrentDllPath("SocketTransceiverLine").c_str(), "CreateTransceiver");
   if (!m_transceiver.IsValid())
   {
      //user_interface::SetOutputText(user_interface::OT_ERROR, "Can't load Socket Transceiver Line!");
      return;
   }
   std::thread thr(&ConnectionManager::initTransceiver, this);
   thr.detach();
   return;
}

void ConnectionManager::createDataShareProvider()
{
   m_shareProvider.Create(SVGUtils::CurrentDllPath("DataShareProvider").c_str(), "CreateDataShareProvider");
   if (!m_shareProvider.IsValid())
   {
      //user_interface::SetOutputText(user_interface::OT_ERROR, "Can't load Socket Transceiver Line!");
      return;
   }
}

void ConnectionManager::initTransceiver()
{
   m_transceiver->Init(m_info);
}

void ConnectionManager::callback(const char* text)
{
   m_infoboard->SetOutputText(text);
}

void ConnectionManager::parseIncomingCommand(const char* data)
{
   std::string str(data);
   if (str.find(".") != std::string::npos)
   {
      //m_shareProvider->GetShared(str.capacitystr(), meta, &m_rawdata);
   }
}
