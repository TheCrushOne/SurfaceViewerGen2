#pragma once

#include "SurfaceInterfaces.h"
#include <functional>

struct iTransceiver : colreg::iReleasable
{
   virtual void Init(const char* serverAddr, const char* serverPort, const char* clientPort, std::function<void(const char*)> traceCallback, std::function<void(const char*)> dataCallback) = 0;
   virtual void Send(const char* message) = 0;
};

extern "C" EXPRTIMPRT iTransceiver * CreateTransceiver();