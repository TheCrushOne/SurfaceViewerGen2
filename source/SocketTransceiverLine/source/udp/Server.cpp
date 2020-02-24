#include "stdafx.h"
#include "Server.h"
#include "udef.h"
#include "JsonCommandSplitter.h"

Server::Server(const transceiver::transceiver_info& info)
   : m_info(info)
{}

void Server::Init()
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
      m_info.trace_callback(("WSAStartup failed with error: %d\n" + std::to_string(iResult)).c_str());
      return;
   }

   ZeroMemory(&hints, sizeof(hints));
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_protocol = IPPROTO_TCP;
   hints.ai_flags = AI_PASSIVE;

   // Resolve the server address and port
   iResult = getaddrinfo(NULL, m_info.server_port.c_str(), &hints, &result);
   if (iResult != 0)
   {
      m_info.trace_callback(("getaddrinfo failed with error: " + std::to_string(iResult)).c_str());
      WSACleanup();
      return;
   }

   // Create a SOCKET for connecting to server
   ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
   if (ListenSocket == INVALID_SOCKET) {
      m_info.trace_callback(("socket failed with error: " + std::to_string(WSAGetLastError())).c_str());
      freeaddrinfo(result);
      WSACleanup();
      return;
   }

   // Setup the TCP listening socket
   iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
   if (iResult == SOCKET_ERROR)
   {
      m_info.trace_callback(("bind failed with error: " + std::to_string(WSAGetLastError())).c_str());
      freeaddrinfo(result);
      closesocket(ListenSocket);
      WSACleanup();
      return;
   }

   freeaddrinfo(result);

   iResult = listen(ListenSocket, SOMAXCONN);
   if (iResult == SOCKET_ERROR)
   {
      m_info.trace_callback(("listen failed with error: " + std::to_string(WSAGetLastError())).c_str());
      closesocket(ListenSocket);
      WSACleanup();
      return;
   }

   // Accept a client socket
   ClientSocket = accept(ListenSocket, NULL, NULL);
   if (ClientSocket == INVALID_SOCKET)
   {
      m_info.trace_callback(("accept failed with error: " + std::to_string(WSAGetLastError())).c_str());
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
         m_info.trace_callback(("Bytes received: " + std::to_string(iResult)).c_str());
         auto token = JsonCommandSplitter::GetCommandToken(recvbuf);
         if (m_info.data_callback_map.find(token) != m_info.data_callback_map.end())
            m_info.data_callback_map.at(token)(recvbuf);
         // Echo the buffer back to the sender
         //iSendResult = send(ClientSocket, recvbuf, iResult, 0);
         //if (iSendResult == SOCKET_ERROR)
         //{
         //   m_traceCallback(("send failed with error: " + std::to_string(WSAGetLastError())).c_str());
         //   closesocket(ClientSocket);
         //   WSACleanup();
         //   return;
         //}
         //printf("Bytes sent: %d\n", iSendResult);
      }
      else if (iResult == 0)
         m_info.trace_callback("Connection 0\n");
      else
      {
         m_info.trace_callback(("recv failed with error: " + std::to_string(WSAGetLastError())).c_str());
         closesocket(ClientSocket);
         WSACleanup();
         return;
      }

   } while (/*iResult > 0*/true);

   // shutdown the connection since we're done
   iResult = shutdown(ClientSocket, SD_SEND);
   if (iResult == SOCKET_ERROR)
   {
      m_info.trace_callback(("shutdown failed with error: " + std::to_string(WSAGetLastError())).c_str());
      closesocket(ClientSocket);
      WSACleanup();
      return;
   }

   // cleanup
   closesocket(ClientSocket);
   WSACleanup();

   return;
}