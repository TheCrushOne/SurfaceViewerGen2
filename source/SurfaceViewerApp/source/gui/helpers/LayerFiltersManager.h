#pragma once
#include "common\base_class.h"
#include "colreg/LayerVisibilityControl.h"

namespace SV
{
   /*struct filter_info
   {
      std::string name;
      bool visible = false;
      std::unordered_map<std::string, filter_info> childs;
   };*/

   /*struct layer_filter_tag
   {
      static constexpr char general[] = "General";

      static constexpr char explications[] = "Explications";
      static constexpr char land[] = "Land";
      static constexpr char air[] = "Air";

      static constexpr char coverages[] = "Coverages";

      static constexpr char step_templ[] = "Step_";
   };*/

   class LayerFiltersManager
      : public Singleton< LayerFiltersManager>
   {
   public:
      const surface_simulation::LayerVisibilityControl* GetFilters()
      {
         prepareFilters();
         return m_filters;
      }

      void Reload() { prepareFilters(); }
      void ShowFilter(const std::vector<std::string>& path, bool show);
      bool IsFilterVisible(const std::vector<std::string>& path)const;

   protected:
      LayerFiltersManager();
      void prepareFilters();
      //filter_info& addLayerFilterNode(filter_info& filter, const std::string& folderName);

   private:
      friend class Singleton<LayerFiltersManager>;
      surface_simulation::LayerVisibilityControl* m_filters = nullptr;
      //filter_info m_filters;
   };
}