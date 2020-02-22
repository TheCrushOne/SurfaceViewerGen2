#pragma once
#include "common\observer.h"
#include "colreg/ModuleGuard.h"
#include "crossdllinterface/ConverterInterface.h"
#include "crossdllinterface\TransceiverInterface.h"
#include "crossdllinterface/DataShareInterface.h"

namespace ColregSimulation
{
   enum class ROUTE_TYPE : char;
}

enum class CSENARIO_STATUS
{
   SS_RUN = 0,
   SS_PAUSE,
   SS_STOP,
   SS_NOT_LOADED,
   UPDATE
};


/**
* \класс ScenarioDispather
* Регистрирует наблюдателей для рассылки уведомлений изменений при работе сценария
  Сигнатура фунукции наблюдателя:
  bool OnScenarioLoad( const char* name )
  bool OnScenarioStatusChanged(CSENARIO_STATUS status)
  bool OnScenarioTimeChanged(double time)
  bool OnScenarioModified();
*/
class ScenarioDispather : public Singleton< ScenarioDispather >
{
public:
   template< class T >
   void AddReciever(std::shared_ptr<T>& pObj)
   {
      std::weak_ptr<T> wptr{ pObj };

      m_observable.Attach(pObj, [wptr](const wchar_t* name)
         {
            auto ptr = wptr.lock();
            if (ptr)return ptr->OnScenarioLoad(name);
            return false;
         });

      m_observable2.Attach(pObj, [wptr](CSENARIO_STATUS status)
         {
            auto ptr = wptr.lock();
            if (ptr)return ptr->OnScenarioStatusChanged(status);
            return false;
         });

      m_observable3.Attach(pObj, [wptr](double time)
         {
            auto ptr = wptr.lock();
            if (ptr)return ptr->OnScenarioTimeChanged(time);
            return false;
         });
      m_observable4.Attach(pObj, [wptr]()
         {
            auto ptr = wptr.lock();
            if (ptr)return ptr->OnScenarioModified();
            return false;
         });

      m_observable5.Attach(pObj, [wptr]()
         {
            auto ptr = wptr.lock();
            if (ptr)return ptr->OnAppQuit();
            return false;
         });
   }

   bool OnScenarioLoad(const wchar_t* name) { return m_observable.Notify(false, name); }
   bool OnScenarioStatusChanged(CSENARIO_STATUS status) { return m_observable2.Notify(false, status); }
   bool OnScenarioTimeChanged(double time) { return m_observable3.Notify(false, time); }
   bool OnScenarioModified() { return m_observable4.Notify(false); }
   bool OnAppQuit() { return m_observable5.Notify(false); }

private:
   ScenarioDispather() = default;
   ScenarioDispather(const ScenarioDispather&) = delete;
   ScenarioDispather& operator=(const ScenarioDispather&) = delete;
   Observable< NoLock, bool, const wchar_t* > m_observable;
   Observable< NoLock, bool, CSENARIO_STATUS  > m_observable2;
   Observable< NoLock, bool, double  > m_observable3;
   Observable< NoLock, bool > m_observable4;
   Observable< NoLock, bool > m_observable5;
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

   bool OnScenarioLoad(const wchar_t* name) { return m_pHolder->OnScenarioLoad(name); }
   bool OnScenarioStatusChanged(CSENARIO_STATUS status) { return m_pHolder->OnScenarioStatusChanged(status); }
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
   virtual bool OnScenarioLoad(const wchar_t* name) { return false; }
   virtual bool OnScenarioStatusChanged(CSENARIO_STATUS status) { return false; }
   virtual bool OnScenarioTimeChanged(double time) { return false; }
   virtual bool OnScenarioModified() { return false; }
   virtual bool OnAppQuit() { return false; };
private:
   friend class ScenarioObserver< ScenarioObserverBase >;
   std::shared_ptr< ScenarioObserver< ScenarioObserverBase >  > m_spObserver;
};



class ScenarioManager
   : public Singleton< ScenarioManager>
{
public:
   ScenarioManager();
public:
   void Open(const wchar_t* name);

   void Run();
   void Pause();
   void Restart();

   void Stop();
   void Step();

   void SetTimeScale(int timeScale)
   {
      _timeScale = timeScale;
      if (_state == CSENARIO_STATUS::SS_RUN)
         setState(CSENARIO_STATUS::SS_RUN, true);
   }
   int GetTimeScale()const { return _timeScale; }

   CSENARIO_STATUS GetState()const { return _state; }

   std::wstring GetScenarioName()const { return _scenarioFile; }

   void ReEstimate();
   bool GetAutoPause() const { return _autoPause; }
   void SetAutoPause(bool autoPause) { _autoPause = autoPause; }
   bool GetRecording() const { return _recording; }
   void SetRecording(bool recording);

   bool GetShowRelations() const { return _showRelations; }
   void SetShowRelations(bool show) { _showRelations = show; }

   std::string GetDangerStatFileName() const { return _dangerStatFileName; }
   void SetDangerStatFileName(std::string name) { _dangerStatFileName = name; }
   bool IsDebugMode() const { return _debugMode; }
   void SetDebugMode(bool debug);
private:
   //ScenarioManager() = default;
   friend class Singleton< ScenarioManager>;
   friend class ScenarioDispather;

   void save(const char* fileName = 0, bool focused = false);
   void setState(CSENARIO_STATUS state, bool force = false);

   void printSolutuins();
   void printEnentsAndSuggestions();

   void createTransceiver();
   void initTransceiver();
   void callback(const char*);
private:
   colreg::ModuleGuard<converter::iConverter> m_converter;
   std::unordered_map<colreg::id_type, bool> _shipVisibility = {};
   bool _autoPause = true;
   CSENARIO_STATUS _state = CSENARIO_STATUS::SS_NOT_LOADED;
   bool _showRelations = false;
   std::string _dangerStatFileName = "";
   bool _debugMode = false;
   int _timeScale = 10;
   bool _recording = false;
   std::wstring _scenarioFile;
   colreg::ModuleGuard<iTransceiver> m_transceiver;
   colreg::ModuleGuard<data_share::iDataShareProvider> m_shareProvider;
   const double** m_rawdata;
};