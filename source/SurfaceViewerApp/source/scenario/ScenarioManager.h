#pragma once
#include "common\observer.h"
#include "colreg/ModuleGuard.h"
#include "crossdllinterface\TransceiverInterface.h"
#include "crossdllinterface/DataShareInterface.h"
#include "crossdllinterface\FileStorageInterface.h"
#include "crossdllinterface\TaskInterface.h"
#include "crossdllinterface/SurfaceViewerOrderingWrapperInterface.h"
#include "common/file_storage.h"
#include "common/transceiver_types.h"
#include "colreg\ColregSimulation.h"
#include "simulator\simulator.h"
#include "navdisp\OrderBase.h"
#include "navdisp\ComService.h"

namespace SV::surface_simulation
{
   enum class ROUTE_TYPE : char;
}

#define OBSERVABLE_ATTACH_NO_ARGS(obs, fun) \
   obs.Attach(pObj, [wptr]() \
   { \
      auto ptr = wptr.lock(); \
      if (ptr)return ptr->##fun(); \
      return false; \
   });

#define OBSERVABLE_ATTACH_ARG(obs, fun, type) \
   obs.Attach(pObj, [wptr](type var) \
   { \
      auto ptr = wptr.lock(); \
      if (ptr)return ptr->##fun(var); \
      return false; \
   });
/**
* \класс ScenarioDispather
* Регистрирует наблюдателей для рассылки уведомлений изменений при работе сценария
  Сигнатура фунукции наблюдателя:
  bool OnScenarioLoad( const char* name )
  bool OnScenarioStatusChanged(CSENARIO_STATUS status)
  bool OnScenarioTimeChanged(double time)
  bool OnScenarioModified();
*/
namespace SV
{
   class ScenarioDispather : public Singleton< ScenarioDispather >
   {
   public:
      template< class T >
      void AddReciever(std::shared_ptr<T>& pObj)
      {
         std::weak_ptr<T> wptr{ pObj };

         //OBSERVABLE_ATTACH_NO_ARGS(m_observableCheckOpened, OnScenarioCheckOpened);
         //OBSERVABLE_ATTACH_NO_ARGS(m_observableMapProcessed, OnScenarioMapProcessed);
         //OBSERVABLE_ATTACH_NO_ARGS(m_observablePathFound, OnScenarioPathFound);
         //OBSERVABLE_ATTACH_NO_ARGS(m_observableOptPathFound, OnScenarioOptPathFound);
         OBSERVABLE_ATTACH_ARG(m_observableSimulationStatusChanged, OnScenarioSimulationStatusChanged, surface_simulation::SIMULATION_STATUS);
         OBSERVABLE_ATTACH_ARG(m_observableScenarioStatusChanged, OnScenarioScenarioStatusChanged, surface_simulation::SCENARIO_STATUS);
         OBSERVABLE_ATTACH_ARG(m_observableTimeChanged, OnScenarioTimeChanged, double);
         OBSERVABLE_ATTACH_NO_ARGS(m_observableModified, OnScenarioModified);
         OBSERVABLE_ATTACH_NO_ARGS(m_observableQuit, OnAppQuit);
      }

      //bool OnScenarioCheckOpened() { return m_observableCheckOpened.Notify(false); }
      //bool OnScenarioMapProcessed() { return m_observableMapProcessed.Notify(false); }
      //bool OnScenarioPathFound() { return m_observablePathFound.Notify(false); }
      //bool OnScenarioOptPathFound() { return m_observableOptPathFound.Notify(false); }
      bool OnScenarioSimulationStatusChanged(surface_simulation::SIMULATION_STATUS status) { return m_observableSimulationStatusChanged.Notify(false, status); }
      bool OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS status) { return m_observableScenarioStatusChanged.Notify(false, status); }
      bool OnScenarioTimeChanged(double time) { return m_observableTimeChanged.Notify(false, time); }
      bool OnScenarioModified() { return m_observableModified.Notify(false); }
      bool OnAppQuit() { return m_observableQuit.Notify(false); }

   private:
      ScenarioDispather() = default;
      ScenarioDispather(const ScenarioDispather&) = delete;
      ScenarioDispather& operator=(const ScenarioDispather&) = delete;
      //Observable<NoLock, bool> m_observableCheckOpened;
      //Observable<NoLock, bool> m_observableMapProcessed;
      //Observable<NoLock, bool> m_observablePathFound;
      //Observable<NoLock, bool> m_observableOptPathFound;
      Observable<NoLock, bool, surface_simulation::SIMULATION_STATUS> m_observableSimulationStatusChanged;
      Observable<NoLock, bool, surface_simulation::SCENARIO_STATUS> m_observableScenarioStatusChanged;
      Observable<NoLock, bool, double> m_observableTimeChanged;
      Observable<NoLock, bool> m_observableModified;
      Observable<NoLock, bool> m_observableQuit;
      friend class Singleton< ScenarioDispather >;
   };

   template< class T >
   class ScenarioObserver : public IObserver, public std::enable_shared_from_this< ScenarioObserver< T > >
   {
   public:
      virtual ~ScenarioObserver() = default;

      void Init(T* pHolder)
      {
         m_pHolder = pHolder;
         ScenarioDispather::GetInstance().AddReciever(shared_from_this());
      }
      //bool OnScenarioCheckOpened() { return m_pHolder->OnScenarioCheckOpened(); }
      //bool OnScenarioMapProcessed() { return m_pHolder->OnScenarioMapProcessed(); }
      //bool OnScenarioPathFound() { return m_pHolder->OnScenarioPathFound(); }
      //bool OnScenarioOptPathFound() { return m_pHolder->OnScenarioOptPathFound(); }
      bool OnScenarioSimulationStatusChanged(surface_simulation::SIMULATION_STATUS status) { return m_pHolder->OnScenarioSimulationStatusChanged(status); }
      bool OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS status) { return m_pHolder->OnScenarioScenarioStatusChanged(status); }
      bool OnScenarioTimeChanged(double time) { return m_pHolder->OnScenarioTimeChanged(time); }
      bool OnScenarioModified() { return m_pHolder->OnScenarioModified(); }
      bool OnAppQuit() { return m_pHolder->OnAppQuit(); };
      static std::shared_ptr< ScenarioObserver< T > > Create() { return std::make_shared<ScenarioObserver< T > >(); }
   private:
      T* m_pHolder;
   };

   /**
   * \class ScenarioObserverBase
   * Базовый класс при наследовании от которого производному классу поступают уведомления об изменениях сценария
   */
   class ScenarioObserverBase abstract
   {
   protected:
      ScenarioObserverBase()
      {
         m_spObserver = ScenarioObserver< ScenarioObserverBase >::Create();
         m_spObserver->Init(this);
      }
      //virtual bool OnScenarioCheckOpened() { return false; }
      //virtual bool OnScenarioMapProcessed() { return false; }
      //virtual bool OnScenarioPathFound() { return false; }
      //virtual bool OnScenarioOptPathFound() { return false; }
      virtual bool OnScenarioSimulationStatusChanged(surface_simulation::SIMULATION_STATUS status) { return false; }
      virtual bool OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS status) { return false; }
      virtual bool OnScenarioTimeChanged(double time) { return false; }
      virtual bool OnScenarioModified() { return false; }
      virtual bool OnAppQuit() { return false; };
   private:
      friend class ScenarioObserver< ScenarioObserverBase >;
      std::shared_ptr< ScenarioObserver< ScenarioObserverBase >  > m_spObserver;
   };

   class ScenarioManager
      : public Singleton< ScenarioManager, central_pack*>
   {
   public:
      ScenarioManager(central_pack*);
   public:
      void CheckOpen(const wchar_t*, std::function<void(void)>);
      void ProcessMap(std::function<void(void)>);
      void ProcessMapObjects(std::function<void(void)>);
      void ProcessPaths(std::function<void(void)>);
      void ProcessOptPaths(std::function<void(void)>);

      void Run();
      void Pause();
      void Restart();

      void Stop();
      void Step();

      void SetTimeScale(int timeScale)
      {
         m_timeScale = timeScale;
         auto sim = simulator::getSimulator();
         if (!sim)
            return;
         if (sim->GetSimulatorSimulationState() == surface_simulation::SIMULATION_STATUS::SS_RUN)
            setState(surface_simulation::SIMULATION_STATUS::SS_RUN, true);
      }
      int GetTimeScale()const { return m_timeScale; }

      std::wstring GetScenarioName() const { return m_pathStorage.meta_path.path; }

      void ReSearch();
      void ReEstimate();
      void LogResearchResult();
      //bool OnScenarioPathFound() { return ScenarioDispather::GetInstance().OnScenarioPathFound(); }

      surface_simulation::SCENARIO_STATUS GetScenarionState()
      {
         auto sim = simulator::getSimulator();
         if (!sim)
            return surface_simulation::SCENARIO_STATUS::SS_NOT_LOADED;
         return sim->GetSimulatorScenarioState();
      }
      surface_simulation::SIMULATION_STATUS GetSimulationState()
      {
         auto sim = simulator::getSimulator();
         if (!sim)
            return surface_simulation::SIMULATION_STATUS::SS_STOP;
         return sim->GetSimulatorSimulationState();
      }

      bool GetAutoPause() const { return m_autoPause; }
      void SetAutoPause(bool autoPause) { m_autoPause = autoPause; }
      bool GetRecording() const { return m_recording; }
      void SetRecording(bool recording);

      bool GetShowRelations() const { return m_showRelations; }
      void SetShowRelations(bool show) { m_showRelations = show; }

      std::string GetDangerStatFileName() const { return m_dangerStatFileName; }
      void SetDangerStatFileName(std::string name) { m_dangerStatFileName = name; }
      bool IsDebugMode() const { return m_debugMode; }
      void SetDebugMode(bool debug);
   private:
      //ScenarioManager() = default;
      friend class Singleton<ScenarioManager, central_pack*>;
      friend class ScenarioDispather;

      void processMapCommand(std::function<void(void)>);
      void processMapObjCommand(std::function<void(void)>);
      void processPathCommand(std::function<void(void)>);
      void processOptPathCommand(std::function<void(void)>);

      void save(const char* fileName = 0, bool focused = false);
      void setState(surface_simulation::SIMULATION_STATUS state, bool force = false);

      void createTransceiver();
      void initTransceiver();
      void callback(const char*);
   private:
      //colreg::ModuleGuard<navigation_dispatcher::iOrder, central_pack*, navigation_dispatcher::iComService*> m_converter;
      bool m_autoPause = true;
      //surface_simulation::SCENARIO_STATUS m_state = surface_simulation::SCENARIO_STATUS::SS_NOT_LOADED;
      bool m_showRelations = false;
      std::string m_dangerStatFileName = "";
      bool m_debugMode = false;
      int m_timeScale = 10;
      bool m_recording = false;

      bool m_mapCommandProcessed = false;
      bool m_pathCommandProcessed = false;
      bool m_optPathCommandProcessed = false;

      std::wstring m_databaseFolder;
      std::wstring m_cacheFolder;

      file_utils::global_path_storage m_pathStorage;

      std::wstring m_svgmCacheFolder = L"\\svgm";
      std::wstring m_objMapCacheFolder = L"\\obj";
      std::wstring m_pathsCacheFolder = L"\\paths";
      std::wstring m_optPathsCacheFolder = L"\\opt_paths";

      system::ModuleGuard<transceiver::iTransceiver> m_transceiver;
      system::ModuleGuard<file_storage::iFileStorageManager> m_fsm;
      system::ModuleGuard<surface_ordering::iOrderingWrapper, central_pack*, const wchar_t*> m_orderingWrapper;
      system::ModuleGuard<navigation_dispatcher::iComService, central_pack*, const char*> m_comService;

      transceiver::transceiver_info m_info;
   };
}