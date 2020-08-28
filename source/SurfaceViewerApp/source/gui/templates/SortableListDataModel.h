#pragma once
#include "stdafx.h"

enum DataValueType : short
{
   DVT_STRING = 0,
   DVT_NUMERIC,
};

struct CListCtrl_ItemData
{
   size_t rowIdx;
   std::pair<std::string, size_t> key;    // ”никальный идентификатор и номер столбца
   CListCtrl_ItemData()
   {}
   CListCtrl_ItemData(size_t rowIdx, std::pair<std::string, size_t> key)
      : rowIdx(rowIdx)
      , key(key)
   {}
};

struct CListCtrl_DataRecord
{
   std::vector<std::string> m_data;
   bool m_checked;

   CListCtrl_DataRecord()
      : m_data({})
      , m_checked(false)
   {}
   CListCtrl_DataRecord(std::vector<std::string>& data)
      : m_data(data)
      , m_checked(false)
   {}
   CListCtrl_DataRecord(std::vector<std::string>& data, bool checked)
      : m_data(data)
      , m_checked(checked)
   {}

   const std::string& GetCellText(size_t col) const
   {
      return m_data.at(col);
   }

   size_t  GetColCount() const { return m_data.size(); }
};

enum CListCtrl_RowStatus : short
{
   RS_INLIST = 0,
   RS_NEW,
   RS_OLD,
};

template<typename ItemData>
struct CListCtrl_MapItem
{
   CListCtrl_DataRecord record;
   CListCtrl_RowStatus status;
   ItemData* item;
};

using model_meta_item = std::pair<std::string, DataValueType>;
using model_meta = std::unordered_map<short, model_meta_item>;
template<template<typename> typename DataRecord, typename ItemData>
using model_data = std::unordered_map<std::string, DataRecord<ItemData>>;

template<template<typename> typename DataRecord, typename ItemData>
class CListCtrl_DataModel
{
   const std::vector<double>& m_columnLayout;
   const model_meta& m_meta;
   model_data<DataRecord, ItemData>& m_records;
   bool m_checkBoxes;

public:
   CListCtrl_DataModel(const model_meta& meta, model_data<DataRecord, ItemData>& records, const std::vector<double>& columnLayout = {}, bool checkBoxes = false)
      : m_records(records)
      , m_meta(meta)
      , m_columnLayout(columnLayout)
      , m_checkBoxes(checkBoxes)
   {}
   CListCtrl_DataModel operator=(CListCtrl_DataModel& old)
   {
      m_records = old.GetRecords();
   }

   const std::string& GetCellText(std::string key, size_t col) const
   {
      if (m_records.find(key) == m_records.end())
         ATLASSERT(false);

      return m_records.at(key).record.GetCellText(col);
   }

   inline const std::vector<double>& GetColumnLayout() const { return m_columnLayout; }
   size_t GetRowCount() const { return m_records.size(); }
   size_t GetColCount() const { return m_records.size() > 0 ? m_records.begin()->second.record.GetColCount() : 0; }
   bool IsCheckBoxes() const { return m_checkBoxes; };
   const std::string GetColTitle(size_t col) const
   {
      return m_meta.find(static_cast<short>(col)) != m_meta.end() ? m_meta.at(static_cast<short>(col)).first : "";
   }
   const DataValueType GetColDataValueType(size_t col) const
   {
      return m_meta.find(static_cast<short>(col)) != m_meta.end() ? m_meta.at(static_cast<short>(col)).second : DVT_STRING;
   }
   model_data<DataRecord, ItemData>& GetRecords() { return m_records; }

   void SetData(model_data<DataRecord, ItemData>& records) { m_records = records; }
};
