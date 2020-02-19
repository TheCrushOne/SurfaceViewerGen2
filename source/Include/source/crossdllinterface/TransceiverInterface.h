#pragma once

#include "SurfaceInterfaces.h"
#include <functional>

struct iTransceiver : colreg::iReleasable
{
   virtual void Init(const char* serverAddr, const char* serverPort, const char* clientPort, std::function<void(const char*)> callback) = 0;
};

extern "C" EXPRTIMPRT iTransceiver * CreateTransceiver();