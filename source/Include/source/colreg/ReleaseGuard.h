#pragma once

namespace colreg
{
   /*
   RAI для iReleasable объектов
   */
   template<typename T>
   class ReleaseGuard
   {
   public:
      ReleaseGuard()
      {}

      ReleaseGuard<T>(ReleaseGuard<T>&& rhs)
      {
         *this = std::move(rhs);
      }

      ReleaseGuard<T>(T* ptr) : _ptr(ptr)
      {}

      ~ReleaseGuard()
      {
         release();
      }

      ReleaseGuard<T>& operator=(T* ptr)
      {
         if (_ptr == ptr)
            return *this;

         release();
         _ptr = ptr;

         return *this;
      }

      ReleaseGuard<T>& operator=(ReleaseGuard<T>&& rhs)
      {
         if (this == &rhs)
            return *this;

         release();
         _ptr = rhs._ptr;
         rhs._ptr = nullptr;

         return *this;
      }

      inline const T* Get() const { return _ptr; }

      inline const T* operator->() const { return _ptr; }
      inline T* operator->() { return _ptr; }

      inline operator const T*() const { return _ptr; }
      inline operator T*() { return _ptr; }

      inline operator bool() const { return _ptr != nullptr; }

      inline void Free() { release(); }

   private:
      void release()
      {
         if (_ptr != nullptr)
            _ptr->Release();
         _ptr = nullptr;
      }

   private:
      T* _ptr = nullptr;
   };
}