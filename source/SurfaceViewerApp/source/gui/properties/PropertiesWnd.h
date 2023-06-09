
#pragma once
#include "gui/selection/SelectedObjectManager.h"
#include "PropComboBox.h"
#include "PropertiesWndBase.h"
#include "gui/layers/RenderLayers.h"

class CPropertiesWnd 
   : public CPropertiesWndBase
   , SelectObjectObserver
   , public LayersContainer
{
  
   // A masked edit control for phone number
  
   
// Construction
public:
   CPropertiesWnd() = default;

   bool OnObjectSelected(iProperty* prop) override;

   void Render(render::iRender*renderer) override;
// Attributes
public:


protected:
	
   PropComboBox m_wndShipsCombo;
   PropComboBox m_wndChartObjectsCombo;
   

// Implementation
public:
	virtual ~CPropertiesWnd() = default;
protected:
   void setFont();
   void adjustLayout(LPRECT rect = 0 ) override;
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnComboChanged();
   afx_msg void OnComboChartChanged();
   afx_msg void OnComboEdited();
	DECLARE_MESSAGE_MAP()

	

	int m_nComboHeight = 0;

private:
   void onChartCombopDrop();
   void reloadUnitList();
   void clearUnitList();
   void addUnitVector(std::vector<std::pair<size_t, std::string>> &&data);
   void fillChartObjectsIds(const std::vector<colreg::id_type>& ids);

private:
   std::unordered_map<size_t, std::pair<size_t, ColregSimulation::UNIT_TYPE>> m_unitIdxMap;
   render::iRender* _renderer = nullptr;
   render::FIND_OBJECT_TYPE   _selectedObjectType = render::FIND_OBJECT_TYPE::FOT_NONE;
};