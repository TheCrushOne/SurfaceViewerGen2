#pragma once

#include "SurfaceInterfaces.h"
#include "common/transceiver_types.h"
#include "common/central_class.h"
#include <functional>

namespace SV::transceiver
{
   struct iTransceiver : iReleasable
   {
      virtual void Init(const transceiver_info&) = 0;
      virtual void Send(JsonCommand, const char*) = 0;
   };
}

#ifdef TRANSCEIVER_DLL
#define TRANSEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define TRANSEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" TRANSEXPRTIMPRT SV::transceiver::iTransceiver * CreateTransceiver(SV::central_pack * pack);