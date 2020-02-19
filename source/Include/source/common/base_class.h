#pragma once

#include <atomic>
#include <thread>
#include <chrono>
#include <memory>
#include <mutex>
#include <vector>
#include <algorithm>

// NOTE: Полезности и плюшки

template< class T >
class CObjectHolder
{
public:
   using TPtr = std::weak_ptr<T>;
   static void SetHolder(TPtr pHolder) { m_pHolder.swap(pHolder); }
   static TPtr GetHolder() { return m_pHolder; }
private:
   static TPtr m_pHolder;
};


class Noncopyable
{
protected:
   Noncopyable() {}
   ~Noncopyable() {}
private:
   Noncopyable(const Noncopyable&) = delete;
   const Noncopyable& operator=(const Noncopyable&) = delete;
};

template< class T >
class Singleton : public  Noncopyable
{
public:
   static T& GetInstance()
   {
      static T instance;
      return instance;
   }

protected:
   Singleton() = default;
   Singleton(const Singleton&) = delete;
   Singleton& operator=(const Singleton&) = delete;

};

template< class T >
class ScopeLock
{
   T m_lock;
public:
   void lock()
   {
      m_lock.lock();
   }
   void unlock()
   {
      m_lock.unlock();
   }
   class CLock {
   public:
      CLock(ScopeLock& sl, bool _bLock = true) :
         m_sl(sl),
         m_bLock(_bLock)
      {
         if (m_bLock)
            m_sl.lock();
      }
      ~CLock() {
         if (m_bLock)
            m_sl.unlock();
      }

   private:
      ScopeLock& m_sl;
      const bool m_bLock;
   };
};

class SpinLock
{
   std::atomic_flag locked = ATOMIC_FLAG_INIT;
public:
   void lock()
   {
      while (locked.test_and_set(std::memory_order_acquire))
      {
         std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
   }
   void unlock()
   {
      locked.clear(std::memory_order_release);
   }
};

class NoLock
{
   std::atomic_flag locked = ATOMIC_FLAG_INIT;
public:
   void lock()
   {

   }
   void unlock()
   {

   }
};

class PerfomanceLock
{
public:
   PerfomanceLock() :_timeDuration{ 0 }
   {}
   PerfomanceLock(const PerfomanceLock& other)
   {
      _timeDuration = other._timeDuration;
   }
   PerfomanceLock& operator = (const PerfomanceLock& other)
   {
      if (&other != this)
         _timeDuration = other._timeDuration;
      return *this;
   }
   inline void lock() { _lockTime = std::chrono::system_clock::now(); }
   inline void unlock() { _timeDuration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - _lockTime); }
   inline void Clear() { _timeDuration = std::chrono::microseconds::zero(); }
   inline auto GetDuration()const { return _timeDuration; }
private:
   std::chrono::time_point<std::chrono::system_clock> _lockTime;
   std::chrono::microseconds _timeDuration;
};

template <typename T, typename Deleter = std::default_delete<T> >
struct ICloneable
{
   using TypePtr = ::std::unique_ptr<T, Deleter>;
   virtual TypePtr Clone() const = 0;
};

template <typename T>
struct ISharedCloneable
{
   using TypePtr = ::std::shared_ptr<T>;
   virtual TypePtr Clone() const = 0;
};

template <typename T, typename Deleter = std::default_delete<T>>
struct cloneable_unique_ptr
   : public std::unique_ptr<T, Deleter>
{
   using base_class = std::unique_ptr<T, Deleter>;
   cloneable_unique_ptr() = default;

   cloneable_unique_ptr(const std::unique_ptr<T>& ptr) = delete;


   cloneable_unique_ptr(base_class&& ptr)noexcept
   {
      base_class::operator =(std::move(ptr));
   }

   cloneable_unique_ptr(T* p)
   {
      reset(p);
   }

   cloneable_unique_ptr& operator=(T* p)
   {
      reset(p);
      return *this;
   }

   cloneable_unique_ptr(const cloneable_unique_ptr& other)
   {
      *this = other;
   }

   cloneable_unique_ptr(cloneable_unique_ptr&& other)noexcept
   {
      base_class::operator =(std::move(other));
   }

   cloneable_unique_ptr& operator=(const cloneable_unique_ptr& other)
   {
      if (this != &other)
      {
         auto cloneResult = other ? other->Clone() : nullptr;

         if constexpr (std::is_pointer< decltype(cloneResult)>::value)
         {
            //result of Clone is raw pointer
            reset(cloneResult);
         }
         else
         {
            //result of Clone is smart_pointer
            base_class::operator = (std::move(cloneResult));

         }
      }
      return *this;
   }

   cloneable_unique_ptr& operator=(cloneable_unique_ptr&& other)noexcept
   {
      if (this != &other)
      {
         base_class::operator = (std::move(other));
      }
      return *this;
   }
};


template< class Value, class SortValue>
class Sorter
{
public:
   struct sort_helper
   {
      Value value;
      SortValue sort_Value;
      inline operator Value()const { return value; }
   };

   inline void Add(Value v, SortValue sv) { vecToSort.emplace_back(sort_helper{ v,sv }); }

   template< typename Pred>
   std::vector<Value> Sort(Pred pred)
   {
      std::sort(vecToSort.begin(), vecToSort.end(), pred);
      return std::vector<Value>{vecToSort.begin(), vecToSort.end()};
   }

private:
   std::vector<sort_helper>vecToSort;
};


template< class T>
struct ArrayHolder
{
   ArrayHolder(const T** p, size_t* sizePtr, size_t size) : pData{ p }, pSize{ sizePtr }, data(size)
   {
      *pData = size ? &data[0] : nullptr;
      *pSize = size;
   }
   ~ArrayHolder()
   {
      *pData = nullptr;
      *pSize = 0;
   }
private:
   const T** pData;
   size_t* pSize;
   const std::vector<T> data;
};


template< class T >
class DataContainer
{
public:
   DataContainer(size_t resizeValue = 16)
      : _curIndex{ 0 }
      , _resizeValue{ resizeValue }
   {}

   inline auto& getNext()
   {
      if (_container.size() <= _curIndex)
         _container.resize(_container.size() + _resizeValue);
      return _container[_curIndex++];
   }
   inline auto Size()const { return _curIndex; }
   inline void Reset() { _curIndex = 0; }
   inline operator T* () { return _container.data(); }
   inline const std::vector<T>& Data()const { return _container; }

private:
   size_t _curIndex;
   const size_t _resizeValue;
   std::vector<T> _container;
};

template< class T >
class AutoContainer
{
public:
   typedef std::vector< T* > ListObjects;
   AutoContainer()
   {
      _objects.push_back((T*)this);
   }
   virtual ~AutoContainer()
   {
      auto itf = std::find(_objects.begin(), _objects.end(), (T*)this);
      if (itf != _objects.end())
         _objects.erase(itf);
   }
   static ListObjects _objects;
};
