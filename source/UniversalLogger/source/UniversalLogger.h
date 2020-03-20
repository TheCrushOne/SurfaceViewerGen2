#pragma once

#include "crossdllinterface/UniversalLoggerInterface.h"

namespace logger
{
   class UniversalLogger : public iUniversalLogger, public Communicable
   {
   public:
      UniversalLogger();

      void Init(ICommunicator* comm) override final;
      bool LogThreadResearchResult(const wchar_t*, const ThreadResearchComplexStorage&) override final;
      void Release() override final { delete this; }
   };
}