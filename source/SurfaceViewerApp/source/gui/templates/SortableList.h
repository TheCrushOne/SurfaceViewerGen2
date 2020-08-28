#pragma once
#include "SortableListDataModel.h"
#include "stdafx.h"
#include "Resource.h"
#include <windows.h>
#include <commctrl.h>


#define BEGIN_NESTED_TEMPLATE_MESSAGE_MAP(theClass, type_name, subtype_name, baseClass) \
   PTM_WARNING_DISABLE \
   template < template<typename> typename type_name, typename subtype_name> \
   const AFX_MSGMAP* theClass< type_name, subtype_name >::GetMessageMap() const \
      { return GetThisMessageMap(); } \
   template < template<typename> typename type_name, typename subtype_name> \
   const AFX_MSGMAP* PASCAL theClass< type_name, subtype_name >::GetThisMessageMap() \
   { \
      typedef theClass< type_name, subtype_name > ThisClass; \
      typedef baseClass TheBaseClass; \
      __pragma(warning(push)) \
      __pragma(warning(disable: 4640)) /* message maps can only be called by single threaded message pump */ \
      static const AFX_MSGMAP_ENTRY _messageEntries[] = \
      {


template<template<typename> typename DataRecord, typename ItemData>
class CListCtrl_Sortable : public CListCtrl
{
   CListCtrl_DataModel<DataRecord, ItemData>& m_dataModel;
   bool m_ascending;
   int  m_sortCol;
   bool m_itemsInTable;
   std::unordered_map<std::string, size_t> m_keyIdxMap;

   DECLARE_MESSAGE_MAP();

   afx_msg BOOL OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg BOOL OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
   void PreSubclassWindow();

public:
   CListCtrl_Sortable()
      : m_dataModel(CListCtrl_DataModel<DataRecord, ItemData>(model_meta(), model_data()))
   {}
   CListCtrl_Sortable(CListCtrl_DataModel<DataRecord, ItemData>& dataModel)
      : m_ascending(false)
      , m_sortCol(-1)
      , m_dataModel(dataModel)
      , m_itemsInTable(false)
   {}

   size_t GetColumnData(size_t col) const;

   inline bool IsAscending() const { return m_ascending; }

   void refillKeyIdxMap();
   void resetRowIndexes();
   void SetSortArrow(size_t col, bool ascending);
   inline void SetDataModel(CListCtrl_DataModel<DataRecord, ItemData>& dataModel) { m_dataModel = dataModel; }

   void LoadData();
   void ResetSortOrder();
   void ResetLayout();
   virtual bool SortColumn(size_t columnIndex, bool ascending);
   void Clear();
   int findValidPlace();
   int findByKey(size_t cidx, std::string key);
   void clearOldRecords();
};

BEGIN_NESTED_TEMPLATE_MESSAGE_MAP(CListCtrl_Sortable, DataRecord, ItemData, CListCtrl)
   ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnHeaderClick)
   ON_NOTIFY_REFLECT_EX(LVN_GETDISPINFO, OnGetDispInfo)
   ON_WM_MOUSEMOVE()
   END_MESSAGE_MAP()

template<template<typename> typename DataRecord, typename ItemData>
BOOL CListCtrl_Sortable<DataRecord, ItemData>::OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult)
{
   // блокируем двойной триггер по нажатию на хедер
   static bool ignoreNextChange = false;
   if (ignoreNextChange)
   {
      ignoreNextChange = false;
      return FALSE;
   }
   ignoreNextChange = true;
   NMLISTVIEW* pLV = reinterpret_cast<NMLISTVIEW*>(pNMHDR);

   SetFocus();

   int colIndex = pLV->iSubItem;

   if (m_sortCol == colIndex)
      m_ascending = !m_ascending;
   else
   {
      m_sortCol = colIndex;
      m_ascending = true;
   }

   if (SortColumn(m_sortCol, m_ascending))
      SetSortArrow(m_sortCol, m_ascending);

   return FALSE;
}

template<template<typename> typename DataRecord, typename ItemData>
void CListCtrl_Sortable<DataRecord, ItemData>::refillKeyIdxMap()
{
   m_keyIdxMap.clear();
   for (size_t idx = 0; idx < GetItemCount(); idx++)
      m_keyIdxMap[reinterpret_cast<ItemData*>(GetItemData(idx))->key] = idx;
}

template<template<typename> typename DataRecord, typename ItemData>
void CListCtrl_Sortable<DataRecord, ItemData>::resetRowIndexes()
{
   for (size_t rowId = 0; rowId < m_dataModel.GetRowCount(); ++rowId)
      reinterpret_cast<ItemData*>(GetItemData(rowId))->rowIdx = rowId;
}

template<template<typename> typename DataRecord, typename ItemData>
void CListCtrl_Sortable<DataRecord, ItemData>::SetSortArrow(size_t colIndex, bool ascending)
{
   UINT bitmapID = m_ascending ? IDR_DOWNARROW : IDR_UPARROW;
   for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
   {
      HDITEM hditem = { 0 };
      hditem.mask = HDI_BITMAP | HDI_FORMAT;
      GetHeaderCtrl()->GetItem(i, &hditem);
      if (hditem.fmt & HDF_BITMAP && hditem.fmt & HDF_BITMAP_ON_RIGHT)
      {
         if (hditem.hbm)
         {
            DeleteObject(hditem.hbm);
            hditem.hbm = NULL;
         }
         hditem.fmt &= ~(HDF_BITMAP | HDF_BITMAP_ON_RIGHT);
         CListCtrl::GetHeaderCtrl()->SetItem(i, &hditem);
      }
      if (i == colIndex)
      {
         hditem.fmt |= HDF_BITMAP | HDF_BITMAP_ON_RIGHT;
         hditem.hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
         ATLASSERT(hditem.hbm != NULL);
         CListCtrl::GetHeaderCtrl()->SetItem(i, &hditem);
      }
   }
   resetRowIndexes();
}

template<template<typename> typename DataRecord, typename ItemData>
void CListCtrl_Sortable<DataRecord, ItemData>::PreSubclassWindow()
{
   CListCtrl::PreSubclassWindow();

   SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
   SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
   SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);
}

template<template<typename> typename DataRecord, typename ItemData>
void CListCtrl_Sortable<DataRecord, ItemData>::ResetSortOrder()
{
   m_ascending = true;
   m_sortCol = -1;
   SetSortArrow(m_sortCol, m_ascending);
}

template<template<typename> typename DataRecord, typename ItemData>
size_t CListCtrl_Sortable<DataRecord, ItemData>::GetColumnData(size_t col) const
{
   LVCOLUMN lvc = { 0 };
   lvc.mask = LVCF_SUBITEM;
   VERIFY(GetColumn(col, &lvc));
   return lvc.iSubItem;
}

template<template<typename> typename DataRecord, typename ItemData>
BOOL CListCtrl_Sortable<DataRecord, ItemData>::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
   NMLVDISPINFO* pNMW = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
   int nItem = pNMW->item.iItem;
   int nSubItem = pNMW->item.iSubItem;

   if (nItem < 0 || nItem >= GetItemCount())
      return FALSE;

   if (nSubItem < 0 || nSubItem >= GetHeaderCtrl()->GetItemCount())
      return FALSE;

   int col = GetColumnData(nSubItem);
   ItemData* itemdata = reinterpret_cast<ItemData*>(GetItemData(nItem));
   std::string key = itemdata->key.first;

   if (pNMW->item.mask & LVIF_TEXT)
   {
      const std::string& result = m_dataModel.GetCellText(key, col);
      std::wstring wresult = SVGUtils::stringToWstring(result.c_str());
      wcsncpy_s(pNMW->item.pszText, MAX_PATH, wresult.c_str(), pNMW->item.cchTextMax);
   }

   return FALSE;
}

namespace {
   template<template<typename> typename DataRecord, typename ItemData>
   struct PARAMSORT
   {
      PARAMSORT(const CListCtrl_DataModel<DataRecord, ItemData>& datamodel, int columnData, bool ascending, CListCtrl* control)
         :m_dataModel(datamodel)
         , m_columnIdx(columnData)
         , m_ascending(ascending)
         , m_control(control)
      {}

      const CListCtrl_DataModel<DataRecord, ItemData>& m_dataModel;
      int  m_columnIdx;
      bool m_ascending;
      CListCtrl* m_control;
   };

   // Comparison extracts values from the DataModel
   template<template<typename> typename DataRecord, typename ItemData>
   int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
   {
      PARAMSORT<DataRecord, ItemData>& ps = *(PARAMSORT<DataRecord, ItemData>*)lParamSort;
      ItemData* item1 = reinterpret_cast<ItemData*>(lParam1);
      ItemData* item2 = reinterpret_cast<ItemData*>(lParam2);
      const CListCtrl_DataModel<DataRecord, ItemData>& model = ps.m_dataModel;
      CListCtrl* control = ps.m_control;
      CString left = control->GetItemText(item1->rowIdx, ps.m_columnIdx);
      CString right = control->GetItemText(item2->rowIdx, ps.m_columnIdx);
      std::wstring wcleft(left.GetString());
      std::wstring wcright(right.GetString());
      std::string cleft = SVGUtils::wstringToString(wcleft);
      std::string cright = SVGUtils::wstringToString(wcright);
      if (model.GetColDataValueType(ps.m_columnIdx) == DVT_NUMERIC)
         return ps.m_ascending ? atof(cleft.c_str()) >= atof(cright.c_str()) : atof(cleft.c_str()) < atof(cright.c_str());
      else
         return ps.m_ascending ? _tcscmp(left, right) : _tcscmp(right, left);
      ATLASSERT(false);
   }
}

template<template<typename> typename DataRecord, typename ItemData>
bool CListCtrl_Sortable<DataRecord, ItemData>::SortColumn(size_t columnIndex, bool ascending)
{
   if (GetItemCount() != m_dataModel.GetRowCount())
      return false;

   PARAMSORT<DataRecord, ItemData> paramsort(m_dataModel, columnIndex, ascending, this);
   CListCtrl::SortItems(SortFunc<DataRecord, ItemData>, (DWORD_PTR)&paramsort);
   return true;
}

template<template<typename> typename DataRecord, typename ItemData>
void CListCtrl_Sortable<DataRecord, ItemData>::LoadData()
{
   LockWindowUpdate();
   if (GetHeaderCtrl()->GetItemCount() == 0)
   {
      // Create Columns
      for (size_t col = 0; col < m_dataModel.GetColCount(); ++col)
      {
         const std::string& title = m_dataModel.GetColTitle(col);
         std::wstring wtitle = SVGUtils::stringToWstring(title);
         bool ins = InsertColumn(col, wtitle.c_str(), LVCFMT_LEFT, 100) != -1;
         ATLASSERT(ins);
      }
   }

   SetRedraw(FALSE);
   // в списке есть поля со статусами RS_NEW и RS_INLIST
   for (auto& rec : m_dataModel.GetRecords())
   {
      int idx = -1;
      if (rec.second.status == RS_NEW)
      {
         idx = findValidPlace();
         if (idx == -1)
         {
            idx = GetItemCount();
            InsertItem(idx, L"");
         }
         if (m_dataModel.IsCheckBoxes())
            SetCheck(idx, true);
         rec.second.status = RS_INLIST;
      }
      else
      {
         ATLASSERT(rec.second.status == RS_INLIST);
         idx = findByKey(rec.second.item->key.second, rec.second.item->key.first);
         // перезапись item'a
         if (idx != -1)
            delete reinterpret_cast<ItemData*>(GetItemData(idx));
         else
         {
            ATLASSERT(false);
            continue;
         }
      }
      SetItemData(idx, reinterpret_cast<DWORD_PTR>(rec.second.item));
      for (size_t col = 0; col < m_dataModel.GetColCount(); ++col)
      {
         std::wstring witem = SVGUtils::stringToWstring(rec.second.record.m_data.at(col).c_str());
         SetItemText(idx, col, witem.c_str());
      }
   }
   clearOldRecords();

   ATLASSERT(GetItemCount() == m_dataModel.GetRowCount());

   SetRedraw(TRUE);
   UpdateWindow();
   resetRowIndexes();
   if (m_sortCol != -1)
      SortColumn(m_sortCol, m_ascending);
   UnlockWindowUpdate();
}

template<template<typename> typename DataRecord, typename ItemData>
void CListCtrl_Sortable<DataRecord, ItemData>::Clear()
{
   DeleteAllItems();
   Invalidate();
   UpdateWindow();
}

template<template<typename> typename DataRecord, typename ItemData>
void CListCtrl_Sortable<DataRecord, ItemData>::ResetLayout()
{
   if (!m_dataModel.GetColCount())
      return;
   CRect rect;
   GetClientRect(&rect);
   int DW = rect.right - rect.left;
   const auto& colLt = m_dataModel.GetColumnLayout();
   int multWidth = DW;
   for (size_t idx = 0; idx < m_dataModel.GetColCount(); idx++)
      multWidth -= colLt.at(idx) > 1. ? static_cast<int>(colLt.at(idx)) : 0;
   for (size_t idx = 0; idx < m_dataModel.GetColCount(); idx++)
      SetColumnWidth(idx, static_cast<int>(colLt.at(idx) * (colLt.at(idx) > 1. ? 1. : multWidth)));
}

template<template<typename> typename DataRecord, typename ItemData>
int CListCtrl_Sortable<DataRecord, ItemData>::findValidPlace()
{
   auto rec = m_dataModel.GetRecords();
   for (size_t idx = 0; idx < GetItemCount(); idx++)
   {
      auto key = reinterpret_cast<ItemData*>(GetItemData(idx))->key.first;
      if (rec.find(key) == rec.end())
         return idx;
   }
   return -1;
}

template<template<typename> typename DataRecord, typename ItemData>
void CListCtrl_Sortable<DataRecord, ItemData>::clearOldRecords()
{
   auto rec = m_dataModel.GetRecords();
   for (size_t idx = 0; idx < GetItemCount(); idx++)
   {
      auto key = reinterpret_cast<CListCtrl_ItemData*>(GetItemData(idx))->key.first;
      if (rec.find(key) == rec.end())
      {
         delete reinterpret_cast<ItemData*>(GetItemData(idx));
         DeleteItem(idx--);
      }
   }
}

template<template<typename> typename DataRecord, typename ItemData>
int CListCtrl_Sortable<DataRecord, ItemData>::findByKey(size_t cidx, std::string key)
{
   std::wstring wkey = SVGUtils::stringToWstring(key);
   for (size_t ridx = 0; ridx < GetItemCount(); ridx++)
   {
      if (std::wstring(GetItemText(ridx, cidx).GetString()).compare(wkey) == 0)
         return ridx;
   }
   ATLASSERT(false);
   return -1;
}