#include "stdafx.h"
#include "DebugFiltersManager.h"
//#include "Core\DebugInfoImpl.h"
//#include "colreg\Simulator.h"
#include "colreg\ColregSimulation.h"
#include "simulator/simulator.h"

using namespace SV;

DebugFiltersManager::DebugFiltersManager()
{
   prepareFilters();
}

void DebugFiltersManager::prepareFilters()
{
   m_filters.childs.clear();
   auto* sim = simulator::getSimulator();
   if (!sim)
      return;
   //const auto* debugInfo = sim->GetDebugInfo();
   if (!nullptr/*debugInfo*/)
      return;
   /*const auto roots = debugInfo->GetNodes();
   if (!roots)
      return;
   m_filters.name = L"";

   for (auto& node : *roots)
   {
      for (size_t nodeIdx = 0; nodeIdx < node->subnodes.size; ++nodeIdx)
      {
         addDebugFolder(node->subnodes.arr[nodeIdx], m_filters);
      }
   }
   */
}

void DebugFiltersManager::addDebugFolder(const dbg::debug_info_node& node, filter_info& filter)
{
   //if (wcslen(node.name) > 0)
   //{
   //   filter.childs[node.name].name = node.name;

   //   for (size_t nodeIdx = 0; nodeIdx < node.subnodes.size; ++nodeIdx)
   //      addDebugFolder(node.subnodes.arr[nodeIdx], filter.childs[node.name]);
   //}
}

void DebugFiltersManager::ShowFilter(const std::vector<std::wstring>& path, bool show)
{
   //filter_info* filter = &m_filters;
   //for (const auto& name : path)
   //{
   //   const auto itf = filter->childs.find(name);
   //   ATLASSERT(itf != filter->childs.end());
   //   if (show)
   //      filter->visible = show;
   //   filter = &(*itf).second;
   //}

   //filter->visible = show;
}

bool DebugFiltersManager::IsFilterVisible(const std::vector<std::wstring>& path)const
{
   const filter_info* filter = &m_filters;
   for (const auto& name : path)
   {
      const auto itf = filter->childs.find(name);
      if (itf != filter->childs.end())
         filter = &(*itf).second;
      else
         return false;
   }

   return filter->visible;
}