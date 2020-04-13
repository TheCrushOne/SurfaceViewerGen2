#pragma once

#include "SurfaceInterfaces.h"
#include "common/transceiver_types.h"
#include <functional>

namespace transceiver
{
   struct iTransceiver : colreg::iReleasable
   {
      virtual void Init(const transceiver_info&) = 0;
      virtual void Send(transceiver::JsonCommand, const char*) = 0;
   };
}

#ifdef TRANSCEIVER_DLL
#define TRANSEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define TRANSEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" TRANSEXPRTIMPRT transceiver::iTransceiver * CreateTransceiver();