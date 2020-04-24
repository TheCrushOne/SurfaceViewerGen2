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
      MT_INFO = 0,
      MT_ERROR = 1,
      MT_WARNING = 2,
      MT_DEBUG = 3,
      MT_OUTPUT = 4,
      MT_PERFORMANCE = 5,
      // MS_Progress и MS_InProgress могут комбинироваться с MS_AbortSupport
      MT_EndProgress = 6,
      MT_Progress = 8,
      MT_InProgress = 16,
      MT_AbortSupport = 32
   };
   virtual bool stdCallConv Message(MessageType t, const char* msg, ...) = 0;
   virtual void stdCallConv SetProgress(unsigned int progress) = 0;  // 0..100
   virtual bool stdCallConv GetTerminateFlag() = 0;
   virtual bool stdCallConv UpdateUI() = 0;
   virtual bool stdCallConv RaiseError() = 0;
   virtual bool stdCallConv OnPathFound() = 0;
};