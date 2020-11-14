#pragma once
#include "common\base_class.h"

namespace SV
{
   struct filter_info
   {
      std::string name;
      bool visible = false;
      std::unordered_map<std::string, filter_info> childs;
   };

   struct debug_filter_tag
   {
      static constexpr char general[] = "General";

      static constexpr char explications[] = "Explications";
      static constexpr char land[] = "Land";
      static constexpr char air[] = "Air";

      static constexpr char coverages[] = "Coverages";

      static constexpr char step_templ[] = "Step_";
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

      void Reload() { prepareFilters(); }
      void ShowFilter(const std::vector<std::string>& path, bool show);
      bool IsFilterVisible(const std::vector<std::string>& path)const;

   private:
      DebugFiltersManager();
      void prepareFilters();
      filter_info& addDebugNode(filter_info& filter, const std::string& folderName);

   private:
      friend class Singleton<DebugFiltersManager>;
      filter_info m_filters;
   };
}