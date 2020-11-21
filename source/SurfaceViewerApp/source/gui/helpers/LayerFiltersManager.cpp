#include "stdafx.h"
#include "LayerFiltersManager.h"
#include "simulator/simulator.h"

using namespace SV;

LayerFiltersManager::LayerFiltersManager()
{
   Reload();
}

void LayerFiltersManager::prepareFilters()
{
   m_filters.childs.clear();
   auto* sim = simulator::getSimulator();
   if (!sim)
      return;
   //const auto* debugInfo = sim->GetDebugInfo();

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
   m_filters.name = layer_filter_tag::general;
   auto& explications = addDebugNode(m_filters, layer_filter_tag::explications);
   addDebugNode(explications, layer_filter_tag::land);
   addDebugNode(explications, layer_filter_tag::air);

   auto& coverages = addDebugNode(m_filters, layer_filter_tag::coverages);
   auto covCount = sim->GetState().GetCoverageHistory().size();
   for (size_t step = 0; step < covCount; step++)
      addDebugNode(coverages, std::string(layer_filter_tag::step_templ) + std::to_string(step));
}

filter_info& LayerFiltersManager::addDebugNode(filter_info& filter, const std::string& folderName)
{
   filter.childs[folderName].name = folderName;
   return filter.childs[folderName];
}

void LayerFiltersManager::ShowFilter(const std::vector<std::string>& path, bool show)
{
   filter_info* filter = &m_filters;
   for (const auto& name : path)
   {
      const auto itf = filter->childs.find(name);
      ATLASSERT(itf != filter->childs.end());
      if (show)
         filter->visible = show;
      filter = &(*itf).second;
   }

   filter->visible = show;
}

bool LayerFiltersManager::IsFilterVisible(const std::vector<std::string>& path)const
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