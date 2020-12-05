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
   //m_filters.childs.clear();
   auto* sim = simulator::getSimulator();
   if (!sim)
      return;
   auto* info = sim->GetLayerVisibilityInfo();
   m_filters = info;

   //addLayerFilterNode(m_filters, info);
   
   /*m_filters.name = layer_filter_tag::general;
   auto& explications = addDebugNode(m_filters, layer_filter_tag::explications);
   addDebugNode(explications, layer_filter_tag::land);
   addDebugNode(explications, layer_filter_tag::air);

   auto& coverages = addDebugNode(m_filters, layer_filter_tag::coverages);
   auto covCount = sim->GetState().GetCoverageHistory().size();
   for (size_t step = 0; step < covCount; step++)
      addDebugNode(coverages, std::string(layer_filter_tag::step_templ) + std::to_string(step));*/
}

/*filter_info& LayerFiltersManager::addLayerFilterNode(filter_info& filter, const std::string& folderName)
{
   filter.childs[folderName].name = folderName;
   return filter.childs[folderName];
}*/

void LayerFiltersManager::ShowFilter(const layer_filter_path& path, bool show)
{
   auto* sim = simulator::getSimulator();
   if (!sim)
      return;
   auto* filter = sim->GetLayerVisibilityInfoModify();
   for (const auto& name : path)
   {
      const auto itf = filter->children.find(name);
      ATLASSERT(itf != filter->children.end());
      if (show)
         filter->value = show;
      filter = &(*itf).second;
   }

   filter->value = show;
}

bool LayerFiltersManager::IsFilterVisible(const layer_filter_path& path)const
{
   const auto* filter = m_filters;
   for (const auto& name : path)
   {
      const auto itf = filter->children.find(name);
      if (itf != filter->children.end())
         filter = &(*itf).second;
      else
         return false;
   }

   return filter->value;
}