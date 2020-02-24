#pragma once

#include "SurfaceInterfaces.h"
#include "common/transceiver_types.h"
#include <functional>

namespace transceiver
{
   struct iTransceiver : colreg::iReleasable
   {
      virtual void Init(const transceiver_info& info) = 0;
      virtual void Send(transceiver::JsonCommand command, const char* message) = 0;
   };
}

extern "C" EXPRTIMPRT transceiver::iTransceiver * CreateTransceiver();