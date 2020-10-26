#pragma once

#include <map>
#include "gui/selection/SelectedObjectManager.h"
#include "properties/PropertyModify.h"

namespace SV
{
   class CPropertiesCtrl : public CMFCPropertyGridCtrl
   {
   public:
      void ShowProperties(iPropertyInterface* property, bool fullReload = false);
   private:
      CMFCPropertyGridProperty* createMaskedProperty(iPropertyInterface* const& child);
      virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
   private:
      struct prop_item;
      using props_structure = std::map< std::string, prop_item >;

      struct prop_item
      {
         prop_item(CMFCPropertyGridProperty* gProp, PROPERTY_TYPE pt)
            : grid_prop(gProp)
            , prop_type(pt)
         {}

         PROPERTY_TYPE prop_type;
         CMFCPropertyGridProperty* grid_prop = nullptr;
         props_structure prop_childs;
      };
   private:
      void addFolder(iPropertyInterface* folder, CMFCPropertyGridProperty* pFolderProp, props_structure* pStruct);

      props_structure   _prop_struct;
      iPropertyInterface* _selected = nullptr;
   };
}