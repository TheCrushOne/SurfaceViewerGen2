#pragma once

#include <windows.h>
#include <atomic>
#include <chrono>
#include <cassert>
#include <thread>
#undef min
#undef max

class Semaphore
{
private:
   HANDLE m_hSema;

   Semaphore(const Semaphore& other) = delete;
   Semaphore& operator=(const Semaphore& other) = delete;

public:
   Semaphore(int initialCount = 0)
   {
      assert(initialCount >= 0);
      m_hSema = CreateSemaphore(NULL, initialCount, MAXLONG, NULL);
   }

   ~Semaphore()
   {
      CloseHandle(m_hSema);
   }

   void wait()
   {
      WaitForSingleObject(m_hSema, INFINITE);
   }

   void signal(int count = 1)
   {
      ReleaseSemaphore(m_hSema, count, NULL);
   }
};

class NonRecursiveBenaphore
{
private:
   std::atomic<int> m_contentionCount;
   Semaphore m_sema;

public:
   NonRecursiveBenaphore() : m_contentionCount(0) {}

   void lock()
   {
      if (m_contentionCount.fetch_add(1, std::memory_order_acquire) > 0)
      {
         m_sema.wait();
      }
   }

   bool tryLock()
   {
      if (m_contentionCount.load(std::memory_order_relaxed) != 0)
         return false;
      int expected = 0;
      return m_contentionCount.compare_exchange_strong(expected, 1, std::memory_order_acquire);
   }

   void unlock()
   {
      int oldCount = m_contentionCount.fetch_sub(1, std::memory_order_release);
      assert(oldCount > 0);
      if (oldCount > 1)
      {
         m_sema.signal();
      }
   }
};

class RecursiveBenaphore
{
private:
   std::atomic<int> m_contentionCount;
   std::atomic<std::thread::id> m_owner;
   int m_recursion;
   Semaphore m_sema;

public:
   RecursiveBenaphore()
      : m_contentionCount(0)
#if !(defined(__llvm__) && defined(__APPLE__))
      , m_owner(std::thread::id())
#endif
      , m_recursion(0)
   {
#if (defined(__llvm__) && defined(__APPLE__))
      std::atomic_init(&m_owner, std::thread::id());
#endif
      assert(m_owner.is_lock_free());
   }

   void lock()
   {
      std::thread::id tid = std::this_thread::get_id();
      if (m_contentionCount.fetch_add(1, std::memory_order_acquire) > 0)
      {
         if (tid != m_owner.load(std::memory_order_relaxed))
            m_sema.wait();
      }
      //--- We are now inside the lock ---
      m_owner.store(tid, std::memory_order_relaxed);
      m_recursion++;
   }

   bool tryLock()
   {
      std::thread::id tid = std::this_thread::get_id();
      if (m_owner.load(std::memory_order_relaxed) == tid)
      {
         // Already inside the lock
         m_contentionCount.fetch_add(1, std::memory_order_relaxed);
      }
      else
      {
         if (m_contentionCount.load(std::memory_order_relaxed) != 0)
            return false;
         int expected = 0;
         if (!m_contentionCount.compare_exchange_strong(expected, 1, std::memory_order_acquire))
            return false;
         //--- We are now inside the lock ---
         m_owner.store(tid, std::memory_order_relaxed);
      }
      m_recursion++;
      return true;
   }

   void unlock()
   {
      assert(std::this_thread::get_id() == m_owner.load(std::memory_order_relaxed));
      int recur = --m_recursion;
      if (recur == 0)
         m_owner.store(std::thread::id(), std::memory_order_relaxed);
      if (m_contentionCount.fetch_sub(1, std::memory_order_release) > 1)
      {
         if (recur == 0)
            m_sema.signal();
      }
      //--- We are now outside the lock ---
   }
};

class LightweightSemaphore
{
private:
   std::atomic<int> m_count;
   Semaphore m_sema;

   void waitWithPartialSpinning()
   {
      int oldCount;
      // Is there a better way to set the initial spin count?
      // If we lower it to 1000, testBenaphore becomes 15x slower on my Core i7-5930K Windows PC,
      // as threads start hitting the kernel semaphore.
      int spin = 10000;
      while (spin--)
      {
         oldCount = m_count.load(std::memory_order_relaxed);
         if ((oldCount > 0) && m_count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire))
            return;
         std::atomic_signal_fence(std::memory_order_acquire);     // Prevent the compiler from collapsing the loop.
      }
      oldCount = m_count.fetch_sub(1, std::memory_order_acquire);
      if (oldCount <= 0)
      {
         m_sema.wait();
      }
   }

public:
   LightweightSemaphore(int initialCount = 0) : m_count(initialCount)
   {
      assert(initialCount >= 0);
   }

   bool tryWait()
   {
      int oldCount = m_count.load(std::memory_order_relaxed);
      return (oldCount > 0 && m_count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire));
   }

   void wait()
   {
      if (!tryWait())
         waitWithPartialSpinning();
   }

   void signal(int count = 1)
   {
      int oldCount = m_count.fetch_add(count, std::memory_order_release);
      int toRelease = -oldCount < count ? -oldCount : count;
      if (toRelease > 0)
      {
         m_sema.signal(toRelease);
      }
   }
};

using SemaphoreType = RecursiveBenaphore;

inline long time()
{
   static long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
   return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - now;//32607370000000ULL;
}