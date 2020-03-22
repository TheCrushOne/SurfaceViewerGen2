#pragma once

#include "crossdllinterface/UniversalLoggerInterface.h"

namespace logger
{
   class UniversalLogger : public iUniversalLogger
   {
   public:
      UniversalLogger();

      void Init(ICommunicator* comm) override final;
      bool LogThreadResearchResult(const ThreadResearchComplexStorage&) override final;
      void Release() override final { delete this; }
   };
}