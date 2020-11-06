#pragma once
#include <windef.h>
#include "selection/SelectedObjectManager.h"
#include "scenario/ScenarioManager.h"
#include "templates/SortableList.h"

namespace SV
{
   enum PipelineStatusTableColumn : short
   {
      PSTC_STEPID = 0,
      PSTC_STEPNAME,
      PSTC_STATUS,
      PSTC_PERCENT,
   };

   struct PipelineStatusItemData : public CListCtrl_ItemData
   {
      size_t stepIdx;

      PipelineStatusItemData()
      {}
      PipelineStatusItemData(size_t rowIdx, std::pair<std::string, size_t> key, size_t stepIdx)
         : CListCtrl_ItemData(rowIdx, key)
      {}
   };

   class CPipeplineStatusWnd
      : public CDockablePane
      , SelectObjectObserver
      , public ScenarioObserverBase
   {
   public:

   private:
      const model_meta columnMeta = {
         { PSTC_STEPID, { "Ship ID", DVT_NUMERIC } },
         { PSTC_STEPNAME, { "Step name", DVT_STRING } },
         { PSTC_STATUS, { "Status", DVT_STRING } },
         { PSTC_PERCENT, { "Percent", DVT_STRING } }
      };
      const std::unordered_map<bool, std::string> modeNames = {
         { false, "Running" },
         { true, "Done" }
      };
      const std::vector<const char*> obstacleNames = {
         "OT_NONE",
         "OT_CHART_OBJECT",
         "OT_VESSEL",
         "OT_WEATHER_CONDITION",
      };
      /*const std::unordered_map<colreg::SOLUTION_TYPE, COLORREF> solutionColor = {
         { colreg::SOLUTION_TYPE::ST_PASSING, RGB(0,0,255) },
         { colreg::SOLUTION_TYPE::ST_LAST_MOMENT, RGB(255,128,0) },
         { colreg::SOLUTION_TYPE::ST_KEEP_WAY, RGB(0,255,0) },
         { colreg::SOLUTION_TYPE::ST_NOT_FOUND, RGB(255,0,0) },
         { colreg::SOLUTION_TYPE::ST_SELECTING, RGB(153,51,255) },
         { colreg::SOLUTION_TYPE::ST_DANGER_SELECTING, RGB(192,192,192) },
      };*/
   public:
      CPipeplineStatusWnd()
         : m_statusesModel(columnMeta, m_records, columnSize)
         , m_wndStatuses(m_statusesModel)
      {}
      bool OnObjectSelected(iProperty* prop) override;
   protected:
      bool OnScenarioTimeChanged(double time)override;
      //bool OnScenarioStatusChanged(CSENARIO_STATUS status)override;
      //bool OnScenarioLoad(const char* name) override;
      bool OnScenarioModified() override;
      void OnDoubleClickedDangerList(NMHDR* pNMHDR, LRESULT* pResult);
      afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
      afx_msg void OnSize(UINT nType, int cx, int cy);
      afx_msg BOOL OnEraseBkgnd(CDC* pDC);
      afx_msg void OnNMCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);
      DECLARE_MESSAGE_MAP()

   public:
      virtual ~CPipeplineStatusWnd() = default;

   private:
      void clear();
      void update_statuses();
      void AdjustLayout();
      HWND createListView(HWND hWndParent, UINT uId);
   private:
      CListCtrl_Sortable<CListCtrl_MapItem, PipelineStatusItemData> m_wndStatuses;
      CListCtrl_DataModel<CListCtrl_MapItem, PipelineStatusItemData> m_statusesModel;
      model_data<CListCtrl_MapItem, PipelineStatusItemData> m_records;
      // > 1.0 - абсолютные фиксированные значения размера; < 1.0 - процентные от оставшегося пространства
      const std::vector<double> columnSize = { 75.0, 90.0, 0.12, 0.13 };
   };
}