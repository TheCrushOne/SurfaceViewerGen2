#pragma once

#include "colreg/ModuleGuard.h"
#include "crossdllinterface\ResearchEngineInterface.h"
#include "common/central_class.h"
#include "common/research_structs.h"
#include "common/EngineBase.h"

#define CURTIME_MS(time_ms) time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

namespace SV::engine
{
   class ResearchEngine
      : public iResearchEngine
      , public EngineBase
   {
   public:
      ResearchEngine(central_pack* pack);
   public:
      void LaunchResearch(std::function<void(void)>, std::shared_ptr<settings::application_settings>) override final;
      const research::TimeResearchComplexStorage& GetTimeResearchResult() const override final { return m_timeResStorage; }
      const research::LengthResearchComplexStorage& GetLengthResearchResult() const override final { return m_lengthResStorage; }
      const research::ThreadResearchComplexStorage& GetThreadResearchResult() const override final { return m_threadResStorage; }
      const research::task_holder_statistic::experiment_history& GetStatisticHistory() const override final { return m_history; }
      void Release() override final { delete this; }
   private:
      void launchResearch();
      void timeResearch();
      void lengthResearch();
      void threadResearch();

      void generateResMap(size_t mapSize);

      void logThreadResearchResult();

      void threadResNextStep();
      void generateResScenarioData(const settings::research_settings&, const research::ThreadResearchComplexStorage::SuperCell::Index&);
   private:
      // TODO: восстановить
      research::TimeResearchComplexStorage m_timeResStorage;
      research::LengthResearchComplexStorage m_lengthResStorage;

      research::ThreadResearchComplexStorage m_threadResStorage;
      std::function<void(void)> m_endRoundCallback;

      size_t m_threadTaskCurrentIdx;

      std::shared_ptr<pathfinder::path_finder_indata> m_indata;
      research::task_holder_statistic::experiment_history m_history;
   };
}