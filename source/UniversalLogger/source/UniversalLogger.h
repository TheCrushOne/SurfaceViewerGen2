#pragma once

#include "crossdllinterface/UniversalLoggerInterface.h"

#include "json/json_wrapper.h"

namespace SV::logger
{
   class UniversalLogger
      : public iUniversalLogger
      , public Central
   {
      struct tag
      {
         static constexpr char fly_count_values[] = "fly_count_values";
         static constexpr char task_pool_values[] = "task_pool_values";
         static constexpr char thread_pool_values[] = "thread_pool_values";
         static constexpr char length_values[] = "length_values";
      };
   public:
      UniversalLogger(central_pack* pack);

      bool LogThreadResearchResult(const research::ThreadResearchComplexStorage&) override final;
      void Release() override final { delete this; }
   protected:
      void logThreadResearchResult(const research::ThreadResearchComplexStorage&, const std::wstring);
      void logThreadResearchMeta(const research::ThreadResearchComplexStorage&, const std::wstring);
   };
}