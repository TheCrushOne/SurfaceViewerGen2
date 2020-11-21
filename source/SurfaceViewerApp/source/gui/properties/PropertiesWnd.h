
#pragma once
#include "gui/selection/SelectedObjectManager.h"
#include "PropComboBox.h"
#include "PropertiesWndBase.h"
#include "gui/layers/RenderLayers.h"

namespace SV
{
   typedef std::pair<size_t, std::string> DropDownDataElement;
   typedef std::vector<DropDownDataElement> DropDownDataStorage;

   enum class ViewTypeFieldIndex : unsigned short
   {
      VTFI_CHART_OBJS = 0,
      VTFI_UNIT_OBJS,
      VTFI_SETTINGS,
   };

   const static std::unordered_map<ViewTypeFieldIndex, std::string> viewTypeMeta = {
      { ViewTypeFieldIndex::VTFI_CHART_OBJS, "Chart Objs" },
      { ViewTypeFieldIndex::VTFI_UNIT_OBJS, "Unit Objs" },
      { ViewTypeFieldIndex::VTFI_SETTINGS, "Settings" },
   };

   const static std::unordered_map<surface_simulation::PROCESS_STEP_TYPE, std::string> settingsMeta = {
      { surface_simulation::PROCESS_STEP_TYPE::PST_CHECK, "Check-Open" },
      { surface_simulation::PROCESS_STEP_TYPE::PST_MAP, "Map" },
      { surface_simulation::PROCESS_STEP_TYPE::PST_MAP_OBJ, "Map Obj" },
      { surface_simulation::PROCESS_STEP_TYPE::PST_PATHS, "Paths" },
      { surface_simulation::PROCESS_STEP_TYPE::PST_OPT_PATHS, "Opt Paths" },
   };

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

      void Render(render::iRender* renderer) override;
      // Attributes
   public:


   protected:
      CStatic m_viewTypeComboLabel;
      PropComboBox m_viewTypeCombo;

      CStatic m_unitObjectComboLabel;
      PropComboBox m_unitObjectCombo;

      CStatic m_chartObjectComboLabel;
      PropComboBox m_chartObjectsCombo;

      CStatic m_settingsComboLabel;
      PropComboBox m_settingsCombo;

      // Implementation
   public:
      virtual ~CPropertiesWnd() = default;
   protected:
      void setFont();
      void adjustLayout(LPRECT rect = 0) override;
      afx_msg void OnSize(UINT nType, int cx, int cy);
      afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

      afx_msg void OnViewTypeComboChanged();
      afx_msg void OnUnitObjectComboChanged();
      afx_msg void OnChartObjectComboChanged();
      afx_msg void OnSettingsComboChanged();
      
      afx_msg void OnViewTypeComboEdited();
      afx_msg void OnUnitObjectComboEdited();
      afx_msg void OnChartObjectComboEdited();
      afx_msg void OnSettingsComboEdited();

      DECLARE_MESSAGE_MAP()

      int m_nComboHeight = 0;
   private:
      void reloadViewTypeList();
      void reloadUnitObjectList();
      void reloadChartObjectList();
      void reloadSettingsList();

      void clearViewTypeList();
      void clearUnitList();
      void clearChartList();
      void clearSettingsList();

      void addViewTypeVector(std::vector<std::pair<size_t, std::string>>&& data);
      void addUnitObjectVector(std::vector<std::pair<size_t, std::string>>&& data);
      void addChartObjectVector(std::vector<std::pair<size_t, std::string>>&& data);
      void addSettingsVector(std::vector<std::pair<size_t, std::string>>&& data);

      void fillChartObjectsIds(const std::vector<id_type>& ids);
   private:
      std::unordered_map<size_t, std::pair<size_t, surface_simulation::UNIT_TYPE>> m_unitObjectIdxMap;
      render::iRender* _renderer = nullptr;
      render::FIND_OBJECT_TYPE   _selectedObjectType = render::FIND_OBJECT_TYPE::FOT_NONE;
   };
}