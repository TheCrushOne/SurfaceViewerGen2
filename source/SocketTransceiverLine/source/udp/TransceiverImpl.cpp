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

TransceiverImpl::TransceiverImpl()
{}

void TransceiverImpl::Init(const char* serverAddr, const char* serverPort, const char* clientPort, std::function<void(const char*)> traceCallback, std::function<void(const char*)> dataCallback)
{
   m_server = std::make_unique<Server>(serverAddr, serverPort, traceCallback, dataCallback);
   m_client = std::make_unique<Client>(serverAddr, clientPort, traceCallback);
   std::thread sThr(&TransceiverImpl::initServer, this);
   sThr.detach();
   std::thread cThr(&TransceiverImpl::initClient, this);
   cThr.detach();
}

void TransceiverImpl::Send(const char* message)
{
   m_client->Send(message);
}

void TransceiverImpl::initServer()
{
   m_server->Init();
}

void TransceiverImpl::initClient()
{
   m_client->Init();
}

extern "C" iTransceiver* CreateTransceiver()
{
   return new TransceiverImpl();
}