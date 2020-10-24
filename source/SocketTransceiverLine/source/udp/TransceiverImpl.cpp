#include "stdafx.h"
#include "TransceiverImpl.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <future>
#include <string>
#include <mutex>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using namespace SV;

TransceiverImpl::TransceiverImpl(central_pack* pack)
   : Central(pack)
{}

void TransceiverImpl::Init(const transceiver::transceiver_info& info)
{
   m_server = std::make_unique<Server>(info);
   m_client = std::make_unique<Client>(info);
   std::thread sThr(&TransceiverImpl::initServer, this);
   sThr.detach();
   std::thread cThr(&TransceiverImpl::initClient, this);
   cThr.detach();
}

void TransceiverImpl::Send(transceiver::JsonCommand comand, const char* jsonDee)
{
   m_client->Send(jsonDee);
}

void TransceiverImpl::initServer()
{
   m_server->Init();
}

void TransceiverImpl::initClient()
{
   m_client->Init();
}

extern "C" transceiver::iTransceiver* CreateTransceiver(central_pack * pack)
{
   return new TransceiverImpl(pack);
}