#include "stdafx.h"
#include "ConnectionManager.h"
#include "common\utils.h"
#include "utils\infoboard.h"

#include <future>
#include <string>
#include <mutex>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

ConnectionManager::ConnectionManager(std::shared_ptr<Infoboard>& infoboard)
   : m_infoboard(infoboard)
{
   createTransceiver();
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

void ConnectionManager::initTransceiver()
{
   m_transceiver->Init("127.0.0.1", "27015", "8080", [this](const char* txt) { this->callback(txt); });
}

void ConnectionManager::callback(const char* text)
{
   m_infoboard->SetOutputText(text);
}