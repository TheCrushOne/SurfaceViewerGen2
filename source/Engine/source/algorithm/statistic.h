#pragma once

//#include "base/instance.h"
//#include "common/matrix.h"
#include <memory>
#include <vector>

struct StatMeta
{
   //gen 1 & 2
   size_t flyCount;
   //gen 1
   size_t lengthCount;
   //gen 2
   //size_t radiusCount;
};

// Местоположение расчета по индексам осей исследования
// Для обычного запуска пустой скорее всего будет
struct ExperimentMeta
{
   std::vector<size_t> statFieldIdx;
   //size_t flyCount;
   //double maxLength;
   //gen 1 & 2
   //size_t flyCountIdx;
   //gen 1
   //size_t maxLengthIdx;
   //gen 2
   //size_t radarRadiusIdx;
   bool isMulti;
};

// NOTE: Эти 4 ниже только для патфайндера
// Результат одного шага
struct SubTaskStat
{
   ExperimentMeta meta;
   __int64 full;
   __int64 cr;
   __int64 bld;
   __int64 bf;
   size_t psCount;
};

// Результат прогона
struct TaskStat
{
   ExperimentMeta meta;
   __int64 full;
};

template<typename ValType>
struct StatWrapper
{
   //std::shared_ptr<SVM::iMatrix<ValType>> single;
   //std::shared_ptr<SVM::iMatrix<ValType>> multi;
};

struct StatisticDump
{
   StatMeta meta;
   StatWrapper<size_t> pointScoreCount;
   StatWrapper<TaskStat> task;
   StatWrapper<SubTaskStat> subTask;
};

class RuntimeStatistic
{
protected:
   RuntimeStatistic();
public:
   ~RuntimeStatistic();

   void Prepare(size_t, size_t);
   //std::shared_ptr<SVM::iMatrix<TaskStat>> GetTaskStat(bool isMulti) { return isMulti ? m_taskDataM : m_taskDataS; }
   //std::shared_ptr<SVM::iMatrix<SubTaskStat>> GetSubTaskStat(bool isMulti) { return isMulti ? m_subTaskDataM : m_subTaskDataS; }
   //std::shared_ptr<SVM::iMatrix<size_t>> GetPointScoreCountStat() { return m_pointScoreCountStat; }
   //StatisticDump GetStatDump() { return { meta, m_pointScoreCountStat, m_taskData, m_subTaskData }; }
//public slots:
   void StatUpdateSubTaskRun(/*QVariant data*/);
   void StatUpdateTaskRun(/*QVariant data*/);
private:
   StatMeta meta;
   StatWrapper<size_t> m_pointScoreCountStat;
   StatWrapper<TaskStat> m_taskData;
   StatWrapper<SubTaskStat> m_subTaskData;
};
