#pragma once
#include "BaseRef.h"
#include "SimulationStateInterface.h"

namespace SV::surface_simulation
{
   //! ��������� ����������
   struct iSimulator : iReleasable
   {
      //! �������� �������� �������, ������� ����� ����������� �������� �����������
      //virtual dbg::iDebugInfo* GetDebugInfo() const = 0;

      //! ������� �� �������� ���������� ��������
      virtual bool CheckOpenScenario() = 0;

      //! ��������� ������������ ����� �����
      virtual bool LoadProcessedMap() = 0;

      //! ��������� ��������������� ������� �����
      virtual bool LoadProcessedMapObjects() = 0;

      //! ��������� ������������ ����������� ����
      virtual bool LoadProcessedPaths() = 0;

      //! ��������� ������������ ���������������� ����
      virtual bool LoadProcessedOptPaths() = 0;

      //! ������ ����������
      virtual void Start() = 0;

      //! ��������� ���������, ����� ������ � �������� � �������� ���������
      virtual void Stop() = 0;

      //! ��������� ���������, ����� ���� ������ ��� �������� dll
      virtual void Reset() = 0;

      //! ������� �� ���������
      virtual bool IsRunning() const = 0;

      //! ���������� ����������� ����� (���������) ����������
      virtual size_t GetControlPointsCount() const = 0;

      //! �������� ����������� ����� �� �������
      //virtual const control_point_info& GetControlPointInfo(size_t controlPntIdx) const = 0;

      //! ������ ������������ � ����������� �����
      virtual bool PlayFrom(size_t controlPointIdx) = 0;

      //! ��� � ��������� ����������� �����
      virtual bool NextControlPoint() = 0;

      //! ������ ������� ����������� �����
      virtual size_t GetCurrentControlPointIdx() const = 0;

      //! �������� ������ � �������� ����� ��������� ����������
      virtual const iSimulationState& GetState() const = 0;

      //! ���������� ���������� ���� � �����
      virtual bool SaveLogPair(const char* filename) const = 0;

      //! �������� ��� ���������
      virtual const surface_simulation::SIMULATION_PLAYER_TYPE GetSimulationType() = 0;

      //! ���������� ���������
      virtual void SetAppSettings(const settings::application_settings& s) = 0;

      //! ������������� ��������� � xml
      virtual void ReloadSettings() = 0;

      //! ����� ���������
      virtual const settings::application_settings& GetAppSettings() const = 0;

      //! ���������� �����
      virtual void RecountRoutes() = 0;

      //! ���������� ������������
      virtual void RecountResearch() = 0;

      //! ������������ ����������� ������������
      virtual void LogResearchResult() = 0;

      //! ������ ��������
      virtual SCENARIO_STATUS GetSimulatorScenarioState() const = 0;

      //! ������ ���������
      virtual SIMULATION_STATUS GetSimulatorSimulationState() const = 0;

      //! ��������� ������� ��������
      virtual void SetSimulatorScenarioState(SCENARIO_STATUS) = 0;

      //! ��������� ������� ���������
      virtual void SetSimulatorSimulationState(SIMULATION_STATUS) = 0;

      /*!
      \brief �������� ������������ ������� ����� � ���������
      ID ����������� ��� ������� �� GetState (id � objects ���������)
      */
      //virtual colreg::chart_object_id AddDynamicChartObjects(const colreg::chart_object* objects, size_t size) = 0;

      /*!
      \brief �������� ����������� ������� ����� � ���������
      ����� ���������� ����������� ������ ������ �����.
      ID ����������� ��� ������� �� GetState (id � objects ���������)
      */
      //virtual colreg::chart_object_id AddStaticChartObjects(const colreg::chart_object* objects, size_t size) = 0;

      //! ������ ������� ����� �� ���������
      //virtual bool RemoveChartObjects(const colreg::chart_object_id* ids, size_t size) = 0;

      /*!
      \brief ������ ����� ��������� � �������� �������� �����
      ����� ���������� ����������� ������ ������ �����.
      ID ����������� ��� ������� �� GetState (id � data ���������)
      */
      //virtual bool SetChartObjectsData(const colreg::chart_object_id* ids, const colreg::chart_object* data, size_t size) = 0;

      ///////////////////////////statefull//////////////////////////////////////////////////

      //virtual void ReEstimate() = 0;

      //virtual const char* GetSimulationTimeStr() const = 0;
   };
}