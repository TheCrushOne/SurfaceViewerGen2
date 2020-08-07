#pragma once

namespace navigation_dispatcher
{
   struct iConfigDispatcher
   {
      virtual void Dispatch(const wchar_t* configPath) = 0;
   };
}