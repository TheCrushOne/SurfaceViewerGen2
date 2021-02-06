#include "stdafx.h"
#include "PathfinderStatistic.h"

using namespace SV;
using namespace SV::pathfinder;

std::shared_ptr<research::task_holder_statistic::mcmanager_overall_log> PathfinderStatistic::m_stat = std::make_shared<research::task_holder_statistic::mcmanager_overall_log>();

void PathfinderStatistic::Clear()
{
   m_stat->pipeline_run_list.clear();
}

void PathfinderStatistic::AddPipelineStat()
{
   m_stat->pipeline_run_list.emplace_back();
}

void PathfinderStatistic::AddStepStat(size_t unitCount)
{
   auto& current_pipeline = m_stat->pipeline_run_list.back();
   current_pipeline.step_list.emplace_back();
   current_pipeline.step_list.back().unit_count = unitCount;
}

void PathfinderStatistic::AddPacketStat()
{
   auto& current_pipeline = m_stat->pipeline_run_list.back();
   auto& current_step = current_pipeline.step_list.back();
   current_step.packet_list.emplace_back();
}

void PathfinderStatistic::AddTaskStat()
{
   auto& current_pipeline = m_stat->pipeline_run_list.back();
   auto& current_step = current_pipeline.step_list.back();
   auto& current_packet = current_step.packet_list.back();
   current_packet.task_list.emplace_back();
}

void PathfinderStatistic::CommitStatStamp(research::task_holder_statistic::mcmanager_task_log task)
{
   auto& current_pipeline = m_stat->pipeline_run_list.back();
   auto& current_step = current_pipeline.step_list.back();
   auto& current_packet = current_step.packet_list.back();
   current_packet.task_list.at(task.task_idx) = task;
}