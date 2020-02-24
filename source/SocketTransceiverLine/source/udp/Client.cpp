#include "stdafx.h"
#include "Client.h"
#include "udef.h"


Client::Client(const transceiver::transceiver_info& info)
   : m_info(info)
{}

void Client::Send(const char* data)
{
   int iResult;
   iResult = send(m_connectSocket, data, (int)strlen(data) + 1, 0);
   if (iResult == SOCKET_ERROR)
   {
      m_info.trace_callback(("send failed with error: " + std::to_string(WSAGetLastError())).c_str());
      closesocket(m_connectSocket);
      WSACleanup();
      return;
   }
}

void Client::Init()
{
   WSADATA wsaData;
   
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
      m_info.trace_callback(("WSAStartup failed with error: " + std::to_string(iResult)).c_str());
      return;
   }

   ZeroMemory(&hints, sizeof(hints));
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_protocol = IPPROTO_TCP;

   // Resolve the server address and port
   iResult = getaddrinfo(m_info.server_addr.c_str(), m_info.client_port.c_str(), &hints, &result);
   if (iResult != 0)
   {
      m_info.trace_callback(("getaddrinfo failed with error: " + std::to_string(iResult)).c_str());
      WSACleanup();
      return;
   }

   size_t tryCount = 0;
   // Attempt to connect to an address until one succeeds
   while (m_connectSocket == INVALID_SOCKET)
   {
      for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

         // Create a SOCKET for connecting to server
         m_connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
         if (m_connectSocket == INVALID_SOCKET)
         {
            m_info.trace_callback(("socket failed with error: " + std::to_string(WSAGetLastError())).c_str());
            WSACleanup();
            return;
         }

         // Connect to server.
         iResult = connect(m_connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
         if (iResult == SOCKET_ERROR)
         {
            m_info.trace_callback(("socket failed with error: " + std::to_string(WSAGetLastError())).c_str());
            closesocket(m_connectSocket);
            m_connectSocket = INVALID_SOCKET;
            continue;
         }
         break;
      }
      if (m_connectSocket == INVALID_SOCKET)
      {
         m_info.trace_callback(("Unable to connect to server! try " + std::to_string(tryCount)).c_str());
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
   iResult = send(m_connectSocket, sendbuf, (int)strlen(sendbuf), 0);
   if (iResult == SOCKET_ERROR)
   {
      m_info.trace_callback(("send failed with error: " + std::to_string(WSAGetLastError())).c_str());
      closesocket(m_connectSocket);
      WSACleanup();
      return;
   }

   m_info.trace_callback(("Bytes Sent:" + std::to_string(iResult)).c_str());

   // shutdown the connection since no more data will be sent
   //iResult = shutdown(ConnectSocket, SD_SEND);
   //if (iResult == SOCKET_ERROR)
   //{
   //   m_traceCallback(("shutdown failed with error: " + std::to_string(WSAGetLastError())).c_str());
   //   closesocket(ConnectSocket);
   //   WSACleanup();
   //   return;
   //}

   // Receive until the peer closes the connection
   //do {
   //   iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
   //   if (iResult > 0)
   //      m_traceCallback(("Bytes received: " + std::to_string(iResult)).c_str());
   //   else if (iResult == 0)
   //      m_traceCallback("Connection closed");
   //   else
   //      m_traceCallback(("recv failed with error: " + std::to_string(WSAGetLastError())).c_str());
   //} while (iResult > 0);

   // cleanup
   //closesocket(m_connectSocket);
   //WSACleanup();

   return;
}