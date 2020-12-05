#pragma once

#include "ViewTree.h"
#include "helpers/LayerFiltersManager.h"
#include "scenario/ScenarioManager.h"

namespace SV
{
   class CClassToolBar : public CMFCToolBar
   {
      virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
      {
         CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
      }

      virtual BOOL AllowShowOnList() const { return FALSE; }
   };

   class LayerFiltersView
      : public CDockablePane
      , ScenarioObserverBase
   {
   public:
      LayerFiltersView() noexcept;
      virtual ~LayerFiltersView();

      void AdjustLayout();
      void OnChangeVisualStyle();

   protected:
      CClassToolBar m_wndToolBar;
      CViewTree m_treeFilters;
      CImageList m_ClassViewImages;
      UINT m_nCurrSort;

      void fillClassView();
      bool OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS status) override;
      bool OnScenarioSimulationStatusChanged(surface_simulation::SIMULATION_STATUS status) override { return true; }
      bool OnScenarioTimeChanged(double time) override;

      // Overrides
   public:
      virtual BOOL PreTranslateMessage(MSG* pMsg);

   protected:
      afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
      afx_msg void OnSize(UINT nType, int cx, int cy);
      afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
      afx_msg void OnClassAddMemberFunction();
      afx_msg void OnClassAddMemberVariable();
      afx_msg void OnClassDefinition();
      afx_msg void OnClassProperties();
      afx_msg void OnUpdateDebug();
      afx_msg void OnAutoUpdateDebug();
      afx_msg void OnPaint();
      afx_msg void OnSetFocus(CWnd* pOldWnd);
      afx_msg void OnUpdateSort(CCmdUI* pCmdUI);
      afx_msg void OnNMClkTree1(NMHDR* pNMHDR, LRESULT* pResult);
      afx_msg void OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
      afx_msg void OnTvnItemChangedTree(NMHDR* pNMHDR, LRESULT* pResult);
      DECLARE_MESSAGE_MAP()

   private:
      bool onScenarioCheckOpened();
      bool onScenarioMapProcessed();
      bool onScenarioMapObjProcessed();
      bool onScenarioPathFound();
      bool onScenarioOptPathFound();

      void recursive_set_check(HTREEITEM hRoot, bool check);
      void fillSubFilters(HTREEITEM hParent, const surface_simulation::LayerVisibilityControl* filter);

      inline HTREEITEM addChild(HTREEITEM hParent, const char* text)
      {
         auto child = m_treeFilters.GetNextItem(hParent, TVGN_CHILD);
         while (child)
         {
            std::string wname = m_treeFilters.GetItemText(child);
            if (wname == text)
               return child;
            child = m_treeFilters.GetNextItem(child, TVGN_NEXT);
         }
         return m_treeFilters.InsertItem(text, hParent, 0);
      }

      inline std::vector<std::string> getFilterPath(HTREEITEM hItem, bool show, bool showParent)
      {
         std::vector<std::string> path;
         path.push_back(std::string(m_treeFilters.GetItemText(hItem)));

         while (hItem = m_treeFilters.GetParentItem(hItem))
         {
            if (show && showParent)
               m_treeFilters.SetCheck(hItem, show);
            path.push_back(std::string(m_treeFilters.GetItemText(hItem)));
         }

         path.pop_back();
         std::reverse(path.begin(), path.end());
         return path;
      }


      bool showFilter(HTREEITEM hItem, bool show, bool showParent);
   private:
      bool _autoUpdate = false;
   };
}