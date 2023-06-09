#pragma once

#include "crossdllinterface/UniversalLoggerInterface.h"

namespace logger
{
   class UniversalLogger : public iUniversalLogger, public Central
   {
   public:
      UniversalLogger();

      void Init(central_pack* pack) override final;
      bool LogThreadResearchResult(const ThreadResearchComplexStorage&) override final;
      void Release() override final { delete this; }
   protected:
      void logThreadResearchResult(const ThreadResearchComplexStorage&, const std::wstring);
      void logThreadResearchMeta(const ThreadResearchComplexStorage&, const std::wstring);
   };
}