#pragma once
#include "Render\Render.h"
#include "Layers\RenderLayers.h"
#include "scenario/ScenarioManager.h"
#include "Tools\MouseTool.h"

class ScenarioView
   : public CView
   , public ScenarioObserverBase
{
protected: // create from serialization only
   ScenarioView() noexcept;
   DECLARE_DYNCREATE(ScenarioView)

   // Attributes
public:

   render::iRender* GetRenderer() { return _renderer.get(); }
   void InvalidateViewRequest();
   void InvalidateView()
   {
      Invalidate(0);
      UpdateWindow();
   }
   void ShowToolTip(const wchar_t* title, const wchar_t* description);
   void HideToolTip()
   {
      _tipCtrl->Activate(FALSE);
      _tipCtrl->DelTool(this, 0);
   }

   void SetMouseTool(user_interface::EDIT_MODE mode, unsigned long long int userData = 0);

   // Operations
public:

   // Overrides
public:
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
   virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
   virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
   virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

   // Implementation
public:
   virtual ~ScenarioView();

protected:
   bool OnScenarioLoad(const wchar_t* name) override;
   bool OnScenarioModified() override;
   bool OnScenarioStatusChanged(CSENARIO_STATUS status) override;
   bool OnScenarioTimeChanged(double time)override { InvalidateView(); return true; }

   // Generated message map functions
protected:
   afx_msg void OnFilePrintPreview();
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   DECLARE_MESSAGE_MAP()

private:
   void setTimer();
private:
   std::unique_ptr<render::iRender> _renderer;
   std::unique_ptr<LayersContainer> _layers;
   iMouseToolPtr _mouseTool;

   CMFCToolTipCtrl* _tipCtrl;

   bool  _needResetCenter = false;
   bool _needInvalidate = false;
}