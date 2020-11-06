#pragma once
#include <map>
#include "gui/Selection/SelectedObjectManager.h"
#include "properties\PropertyModify.h"

namespace SV
{
   class CPropertiesCtrl
      : public CMFCPropertyGridCtrl
   {
   public:
      void ShowProperties(iProperty* property, bool fullReload = false);

   private:

      CMFCPropertyGridProperty* createMaskedProperty(iProperty* const& child);
      CMFCPropertyGridProperty* createColorProperty(iProperty* const& child);
      virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
   private:

      struct prop_item;
      using props_structure = std::map< std::string, prop_item >;

      struct prop_item
      {
         prop_item(CMFCPropertyGridProperty* gProp, PROPERTY_TYPE pt) : grid_prop(gProp), prop_type(pt) {}

         PROPERTY_TYPE     prop_type;
         CMFCPropertyGridProperty* grid_prop = nullptr;
         props_structure prop_childs;
      };

   private:
      void addFolder(iProperty* folder, CMFCPropertyGridProperty* pFolderProp, props_structure* pStruct);

      props_structure   _prop_struct;
      iProperty* _selected = nullptr;
   };

   class CustomColorProperty : public CMFCPropertyGridColorProperty
   {
   public:
      CustomColorProperty(const CString& strName, const COLORREF& color, CPalette* pPalette = NULL, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0)
         : CMFCPropertyGridColorProperty(strName, color, pPalette, lpszDescr, dwData)
      {}
      void OnClickButton(CPoint point) override
      {
         CColorDialog dlg;
         if (dlg.DoModal() == IDOK)
         {
            COLORREF color = dlg.GetColor();
            SetColor(color);
         }
      }
   };
}