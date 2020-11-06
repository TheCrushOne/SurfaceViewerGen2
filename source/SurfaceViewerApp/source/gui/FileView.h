﻿
#pragma once

#include "ViewTree.h"

namespace SV
{
   class CFileViewToolBar
      : public CMFCToolBar
   {
      virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
      {
         CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
      }

      virtual BOOL AllowShowOnList() const { return FALSE; }
   };

   class CFileView
      : public CDockablePane
   {
      // Создание
   public:
      CFileView() noexcept;

      void AdjustLayout();
      void OnChangeVisualStyle();

      //void CreateScenario();
      //void DeleteScenario();
      //void CreateFolder();
      //void Rename();
      void Simulate();
      // Атрибуты
   protected:
      CViewTree m_treeScenarios;
      CImageList m_FileViewImages;
      CFileViewToolBar m_wndToolBar;
   protected:
      //void FillFileView();

   protected:
      void fillScenariosFiles();
      void addScenarios(HTREEITEM hRoot, const char* dir, bool bAddFolder);

      inline HTREEITEM getItem(HTREEITEM hRoot, const char* dir)
      {
         auto child = m_treeScenarios.GetNextItem(hRoot, TVGN_CHILD);
         while (child)
         {
            std::string name(m_treeScenarios.GetItemText(child));
            if (name == dir)
               return child;
            child = m_treeScenarios.GetNextItem(child, TVGN_NEXT);
         }
         std::string wdir(dir);
         HTREEITEM item = m_treeScenarios.InsertItem(wdir.c_str(), 0, 0, hRoot);
         return item;
      }

      inline std::string getFileName(HTREEITEM hItem)
      {
         std::string res = m_treeScenarios.GetItemText(hItem);

         while (hItem = m_treeScenarios.GetParentItem(hItem))
            res = std::string(m_treeScenarios.GetItemText(hItem)) + "\\" + res;
         return res;
      }

      inline std::string getNewFolderName(HTREEITEM hParent)
      {
         size_t index = 1;
         const std::string newName = "NewFolder_";

         while (true)
         {
            std::stringstream s;
            s << newName.c_str() << index;
            bool find = false;
            auto child = m_treeScenarios.GetNextItem(hParent, TVGN_CHILD);
            while (child)
            {
               std::string name = m_treeScenarios.GetItemText(child);
               if (name == s.str())
               {
                  find = true;
                  break;
               }
               child = m_treeScenarios.GetNextItem(child, TVGN_NEXT);
            }
            if (!find)
               return s.str();
            ++index;
         }
         return newName;
      }


      inline std::string getNewScenarioName(HTREEITEM hParent)
      {
         size_t index = 1;
         const std::string newName = hParent == m_treeScenarios.GetRootItem() ? "Colreg_scenario" : m_treeScenarios.GetItemText(hParent);

         while (true)
         {
            std::stringstream s;
            s << newName.c_str() << "_" << index << ".xml";
            bool find = false;
            auto child = m_treeScenarios.GetNextItem(hParent, TVGN_CHILD);
            while (child)
            {
               std::string name = m_treeScenarios.GetItemText(child);
               if (name == s.str())
               {
                  find = true;
                  break;
               }
               child = m_treeScenarios.GetNextItem(child, TVGN_NEXT);
            }
            if (!find)
               return s.str();
            ++index;
         }
         return newName;
      }
      // Реализация
   public:
      virtual ~CFileView();

   protected:
      afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
      afx_msg void OnSize(UINT nType, int cx, int cy);
      afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
      //afx_msg void OnScenarioCreate();
      //afx_msg void OnScenarioCreateFolder();
      //afx_msg void OnScenarioOpen();
      //afx_msg void OnScenarioDelete();
      //afx_msg void OnScenarioEdit();
      //afx_msg void OnProperties();
      //afx_msg void OnFileOpen();
      //afx_msg void OnFileOpenWith();
      //afx_msg void OnDummyCompile();
      //afx_msg void OnEditCut();
      //afx_msg void OnEditCopy();
      //afx_msg void OnEditClear();
      afx_msg void OnPaint();
      afx_msg void OnSetFocus(CWnd* pOldWnd);
      afx_msg void OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
      afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);

      DECLARE_MESSAGE_MAP()
   };
}