#pragma once

#include <stdio.h>
#include <wchar.h>
#include <cstdlib>
#include <cstdarg>

#ifdef _WIN32
#pragma warning (disable: 4996)
#define stdCallConv __stdcall
#endif
#if defined(__clang__) || defined(__GNUC__)
#define stdCallConv
#endif

struct ICommunicator
{
   enum MessageType
   {
      MS_Info = 0,
      MS_Error = 1,
      MS_Warning = 2,
      MS_Debug = 3,
      // MS_Progress и MS_InProgress могут комбинироваться с MS_AbortSupport
      MS_EndProgress = 4,
      MS_Progress = 8,
      MS_InProgress = 16,
      MS_AbortSupport = 32
   };
   virtual bool stdCallConv Message(MessageType t, const char* msg, ...) = 0;
   virtual void stdCallConv SetProgress(unsigned int progress) = 0;  // 0..100
   virtual bool stdCallConv GetTerminateFlag() = 0;
   virtual bool stdCallConv UpdateUI() = 0;
};