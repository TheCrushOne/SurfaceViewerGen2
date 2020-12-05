#pragma once
#include "common\base_class.h"
#include "colreg/LayerVisibilityControl.h"

typedef std::vector<std::string> layer_filter_path;

namespace SV
{
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
      void ShowFilter(const layer_filter_path& path, bool show);
      bool IsFilterVisible(const layer_filter_path& path)const;

   protected:
      LayerFiltersManager();
      void prepareFilters();
   private:
      friend class Singleton<LayerFiltersManager>;
      const surface_simulation::LayerVisibilityControl* m_filters = nullptr;
   };
}