#include "stdafx.h"
#include "statistic.h"

//#include "base/shared.h"
//#include "common/matrix.h"
//#include "ui/dialog/loadingdlg.h"

RuntimeStatistic::RuntimeStatistic()
{
   //m_taskData.single = WFM::CreateSharedObject<SVM::iMatrix<TaskStat>>(0, 0, TaskStat());
   //m_taskData.multi = WFM::CreateSharedObject<SVM::iMatrix<TaskStat>>(0, 0, TaskStat());
   //m_subTaskData.single = WFM::CreateSharedObject<SVM::iMatrix<SubTaskStat>>(0, 0, SubTaskStat());
   //m_subTaskData.multi = WFM::CreateSharedObject<SVM::iMatrix<SubTaskStat>>(0, 0, SubTaskStat());
   //m_pointScoreCountStat.single = WFM::CreateSharedObject<SVM::iMatrix<size_t>>(0, 0, 0);
   //m_pointScoreCountStat.multi = WFM::CreateSharedObject<SVM::iMatrix<size_t>>(0, 0, 0);
}

RuntimeStatistic::~RuntimeStatistic()
{}

void RuntimeStatistic::StatUpdateSubTaskRun(/*QVariant vdata*/)
{
   //SubTaskStat data = vdata.value<SubTaskStat>();
   //WFM::GetSharedInstance<LoadingDlg>(DBG_DATA)->SubTaskDataUpdate(vdata);
   //qDebug() << "sub task" << data.meta.isMulti;
   //(data.meta.isMulti ? m_subTaskData.multi : m_subTaskData.single)->Set(data.meta.flyCountIdx, data.meta.maxLengthIdx, data);
}

void RuntimeStatistic::StatUpdateTaskRun(/*QVariant vdata*/)
{
   //TaskStat data = vdata.value<TaskStat>();
   //WFM::GetSharedInstance<LoadingDlg>(DBG_DATA)->TaskDataUpdate(vdata);
   //(data.meta.isMulti ? m_taskData.multi : m_taskData.single)->Set(data.meta.flyCountIdx, data.meta.maxLengthIdx, data);
}

void RuntimeStatistic::Prepare(size_t rowCount, size_t colCount)
{
   //meta.flyCount = rowCount;
   //meta.lengthCount = colCount;
   //m_taskData.single->Prepare(rowCount, colCount);
   //m_taskData.multi->Prepare(rowCount, colCount);
   //m_subTaskData.single->Prepare(rowCount, colCount);
   //m_subTaskData.multi->Prepare(rowCount, colCount);
   //m_pointScoreCountStat.single->Prepare(rowCount, colCount);
   //m_pointScoreCountStat.multi->Prepare(rowCount, colCount);
   //WFM::GetSharedInstance<LoadingDlg>(DBG_DATA)->Prepare(rowCount, colCount);
}
