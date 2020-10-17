#pragma once

namespace SV::navigation_dispatcher
{
   struct iConfigDispatcher
   {
      virtual void Dispatch(const char* configPath) = 0;
   };
}