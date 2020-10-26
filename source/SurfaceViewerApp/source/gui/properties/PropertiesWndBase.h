#pragma once
#include "PropertiesCtrl.h"

namespace SV
{
   class CPropertiesToolBar
      : public CMFCToolBar

   {
   public:
      virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
      {
         CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
      }

      virtual BOOL AllowShowOnList() const { return FALSE; }
   };

   // Простое окно с тулбаром и пропертями

   class CPropertiesWndBase
      : public CDockablePane
   {
   public:
      void SetVSDotNetLook(BOOL bSet)
      {
         m_wndPropList.SetVSDotNetLook(bSet);
         m_wndPropList.SetGroupNameFullWidth(bSet);
      }
      void ShowProperties(iPropertyInterface* pProp, bool fullReload = false) { m_wndPropList.ShowProperties(pProp, fullReload); }

      virtual ~CPropertiesWndBase() = default;

   protected:
      CFont& getFont() { return m_fntPropList; }
      virtual void adjustLayout(LPRECT rect = 0);
      void setFont();

      afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
      afx_msg void OnSize(UINT nType, int cx, int cy);
      afx_msg void OnExpandAllProperties();
      afx_msg void OnSortProperties();
      afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
      afx_msg void OnSetFocus(CWnd* pOldWnd);
      afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
      DECLARE_MESSAGE_MAP()

   private:
      void InitPropList();
   private:
      CFont m_fntPropList;
      CPropertiesToolBar m_wndToolBar;
      CPropertiesCtrl m_wndPropList;
   };
}