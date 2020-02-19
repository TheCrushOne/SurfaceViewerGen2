#include "stdafx.h"
#include "TransceiverImpl.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
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

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

TransceiverImpl::TransceiverImpl()
   : m_sAddr(nullptr)
   , m_sPort(nullptr)
   , m_cPort(nullptr)
{}

void TransceiverImpl::Init(const char* serverAddr, const char* serverPort, const char* clientPort, std::function<void(const char*)> callback)
{
   m_callback = callback;
   m_sAddr = serverAddr;
   m_sPort = serverPort;
   m_cPort = clientPort;
   std::thread sThr(&TransceiverImpl::createServer, this);
   sThr.detach();
   std::thread cThr(&TransceiverImpl::createClient, this);
   cThr.detach();
}

void TransceiverImpl::createServer()
{
   WSADATA wsaData;
   int iResult;

   SOCKET ListenSocket = INVALID_SOCKET;
   SOCKET ClientSocket = INVALID_SOCKET;

   struct addrinfo* result = NULL;
   struct addrinfo hints;

   int iSendResult;
   char recvbuf[DEFAULT_BUFLEN];
   int recvbuflen = DEFAULT_BUFLEN;

   // Initialize Winsock
   iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
   if (iResult != 0) {
      m_callback(("WSAStartup failed with error: %d\n" + std::to_string(iResult)).c_str());
      return;
   }

   ZeroMemory(&hints, sizeof(hints));
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_protocol = IPPROTO_TCP;
   hints.ai_flags = AI_PASSIVE;

   // Resolve the server address and port
   iResult = getaddrinfo(NULL, m_sPort, &hints, &result);
   if (iResult != 0)
   {
      m_callback(("getaddrinfo failed with error: " + std::to_string(iResult)).c_str());
      WSACleanup();
      return;
   }

   // Create a SOCKET for connecting to server
   ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
   if (ListenSocket == INVALID_SOCKET) {
      m_callback(("socket failed with error: " + std::to_string(WSAGetLastError())).c_str());
      freeaddrinfo(result);
      WSACleanup();
      return;
   }

   // Setup the TCP listening socket
   iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
   if (iResult == SOCKET_ERROR)
   {
      m_callback(("bind failed with error: " + std::to_string(WSAGetLastError())).c_str());
      freeaddrinfo(result);
      closesocket(ListenSocket);
      WSACleanup();
      return;
   }

   freeaddrinfo(result);

   iResult = listen(ListenSocket, SOMAXCONN);
   if (iResult == SOCKET_ERROR)
   {
      m_callback(("listen failed with error: " + std::to_string(WSAGetLastError())).c_str());
      closesocket(ListenSocket);
      WSACleanup();
      return;
   }

   // Accept a client socket
   ClientSocket = accept(ListenSocket, NULL, NULL);
   if (ClientSocket == INVALID_SOCKET)
   {
      m_callback(("accept failed with error: " + std::to_string(WSAGetLastError())).c_str());
      closesocket(ListenSocket);
      WSACleanup();
      return;
   }

   // No longer need server socket
   closesocket(ListenSocket);

   // Receive until the peer shuts down the connection
   do {

      iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
      if (iResult > 0) {
         m_callback(("Bytes received: " + std::to_string(iResult)).c_str());

         // Echo the buffer back to the sender
         iSendResult = send(ClientSocket, recvbuf, iResult, 0);
         if (iSendResult == SOCKET_ERROR)
         {
            m_callback(("send failed with error: " + std::to_string(WSAGetLastError())).c_str());
            closesocket(ClientSocket);
            WSACleanup();
            return;
         }
         printf("Bytes sent: %d\n", iSendResult);
      }
      else if (iResult == 0)
         m_callback("Connection closing...\n");
      else 
      {
         m_callback(("recv failed with error: " + std::to_string(WSAGetLastError())).c_str());
         closesocket(ClientSocket);
         WSACleanup();
         return;
      }

   } while (/*iResult > 0*/false);

   // shutdown the connection since we're done
   iResult = shutdown(ClientSocket, SD_SEND);
   if (iResult == SOCKET_ERROR)
   {
      m_callback(("shutdown failed with error: " + std::to_string(WSAGetLastError())).c_str());
      closesocket(ClientSocket);
      WSACleanup();
      return;
   }

   // cleanup
   closesocket(ClientSocket);
   WSACleanup();

   return;
}

void TransceiverImpl::createClient()
{
   WSADATA wsaData;
   SOCKET ConnectSocket = INVALID_SOCKET;
   struct addrinfo* result = NULL,
      * ptr = NULL,
      hints;
   const char* sendbuf = "this is a test";
   char recvbuf[DEFAULT_BUFLEN];
   int iResult;
   int recvbuflen = DEFAULT_BUFLEN;

   // Initialize Winsock
   iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
   if (iResult != 0) {
      m_callback(("WSAStartup failed with error: " + std::to_string(iResult)).c_str());
      return;
   }

   ZeroMemory(&hints, sizeof(hints));
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_protocol = IPPROTO_TCP;

   // Resolve the server address and port
   iResult = getaddrinfo(m_sAddr, m_cPort, &hints, &result);
   if (iResult != 0)
   {
      m_callback(("getaddrinfo failed with error: " + std::to_string(iResult)).c_str());
      WSACleanup();
      return;
   }

   size_t tryCount = 0;
   // Attempt to connect to an address until one succeeds
   while (ConnectSocket == INVALID_SOCKET)
   {
      for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

         // Create a SOCKET for connecting to server
         ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
         if (ConnectSocket == INVALID_SOCKET)
         {
            m_callback(("socket failed with error: " + std::to_string(WSAGetLastError())).c_str());
            WSACleanup();
            return;
         }

         // Connect to server.
         iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
         if (iResult == SOCKET_ERROR)
         {
            m_callback(("socket failed with error: " + std::to_string(WSAGetLastError())).c_str());
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
         }
         break;
      }
      if (ConnectSocket == INVALID_SOCKET)
      {
         m_callback(("Unable to connect to server! try " + std::to_string(tryCount)).c_str());
         //WSACleanup();
      }
      tryCount++;
      std::this_thread::sleep_for(std::chrono::seconds(2));
   }

   freeaddrinfo(result);

   //if (ConnectSocket == INVALID_SOCKET)
   //{
   //   m_callback("Unable to connect to server!\n");
   //   WSACleanup();
   //   return;
   //}

   // Send an initial buffer
   iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
   if (iResult == SOCKET_ERROR)
   {
      m_callback(("send failed with error: " + std::to_string(WSAGetLastError())).c_str());
      closesocket(ConnectSocket);
      WSACleanup();
      return;
   }

   m_callback(("Bytes Sent:" + std::to_string(iResult)).c_str());

   // shutdown the connection since no more data will be sent
   iResult = shutdown(ConnectSocket, SD_SEND);
   if (iResult == SOCKET_ERROR)
   {
      m_callback(("shutdown failed with error: " + std::to_string(WSAGetLastError())).c_str());
      closesocket(ConnectSocket);
      WSACleanup();
      return;
   }

   // Receive until the peer closes the connection
   do {
      iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
      if (iResult > 0)
         m_callback(("Bytes received: " + std::to_string(iResult)).c_str());
      else if (iResult == 0)
         m_callback("Connection closed");
      else
         m_callback(("recv failed with error: " + std::to_string(WSAGetLastError())).c_str());
   } while (iResult > 0);

   // cleanup
   closesocket(ConnectSocket);
   WSACleanup();

   return;
}

extern "C" iTransceiver* CreateTransceiver()
{
   return new TransceiverImpl();
}