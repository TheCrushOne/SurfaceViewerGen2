#pragma once
#include <windef.h>
#include "Selection\SelectedObjectManager.h"


class CProgressViewWnd
   : public CDockablePane
   , SelectObjectObserver
   , public ScenarioObserverBase
{
public:
   bool OnObjectSelected(iProperty* prop) override;
protected:

   bool OnScenarioTimeChanged(double time) override;
   void OnReleasedCaptureSlider(NMHDR* pNMHDR, LRESULT* pResult);
   void OnClickedSetButton();
   void OnUpdateSetButton(CCmdUI* pCmdUI);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   DECLARE_MESSAGE_MAP()

public:
   virtual ~CProgressViewWnd() = default;
   // lock drag&drop feature
   virtual BOOL OnBeforeFloat(CRect& /*rectFloat*/, AFX_DOCK_METHOD/*dockMethod*/) { return false; }
   virtual BOOL CanAcceptPane(const CBasePane* pBar) const { return false; }
   bool OnScenarioScenarioStatusChanged(ColregSimulation::SCENARIO_STATUS status) override;
   bool OnScenarioSimulationStatusChanged(ColregSimulation::SIMULATION_STATUS status) override { return true; }
private:
   bool onScenarioCheckOpened();
   bool onScenarioMapProcessed();
   bool onScenarioMapObjProcessed();
   bool onScenarioPathFound();
   bool onScenarioOptPathFound();

   void AdjustLayout();
   void adjustElementLayout();
   inline ColregSimulation::iSimulator* getSimulator();
   inline CTime timestampToTime(double stamp);
   inline double timeToTimestamp(CTime time);
   HWND createSlider(HWND hWndParent, UINT uId);
   HWND createEditBox(HWND hWndParent, UINT uId);
   HWND createButton(HWND hWndParent, UINT uId);
private:
   HWND m_slider;
   HWND m_editbox;
   HWND m_button;
   bool active;
};

