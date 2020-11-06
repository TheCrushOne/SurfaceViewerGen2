#pragma once
#include "common\base_class.h"

namespace SV
{
   namespace dbg
   {
      struct debug_info_node;
   }
   struct filter_info
   {
      std::string name;
      bool visible = false;
      std::unordered_map<std::string, filter_info> childs;
   };

   class DebugFiltersManager
      : public Singleton< DebugFiltersManager>
   {
   public:
      const filter_info& GetFilters()
      {
         prepareFilters();
         return m_filters;
      }

      void ShowFilter(const std::vector<std::string>& path, bool show);
      bool IsFilterVisible(const std::vector<std::string>& path)const;

   private:
      DebugFiltersManager();
      void prepareFilters();
      void addDebugFolder(const dbg::debug_info_node& node, filter_info& filter);

   private:
      friend class Singleton<DebugFiltersManager>;
      filter_info m_filters;
   };
}