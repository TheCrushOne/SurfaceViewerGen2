#pragma once
#include "ReleaseGuard.h"

namespace colreg
{
   template<typename TInterface, typename... TCreationArgs>
   class ModuleGuard
   {
   public:
      ~ModuleGuard()
      {
         _guard.Free();
         if (_handle)
            FreeLibrary(_handle);
      }

      bool Create(const char* dllPath, const char* creationProcName, TCreationArgs... creationProcArgs)
      {
         _handle = LoadLibraryA(dllPath);
         if (!_handle)
         {
            ATLASSERT(!"ModuleGuard: DLL loading failed!");
            return false;
         }

         const auto creationProc = reinterpret_cast<TInterface* (*)(TCreationArgs...)>(GetProcAddress(_handle, creationProcName));
         if (!creationProc)
         {
            ATLASSERT(!"ModuleGuard: Getting creation process failed!");
            return false;
         }

         _guard = creationProc(creationProcArgs...);
         return _guard != nullptr;
      }

      ModuleGuard<TInterface, TCreationArgs...>& operator=(ModuleGuard<TInterface, TCreationArgs...>&& rhs)
      {
         if (this == &rhs)
            return *this;

         _guard = std::move(rhs._guard);
         //rhs._guard = nullptr;

         return *this;
      }

      bool IsValid() const
      {
         return _handle && _guard;
      }

      operator bool() const { return _handle && _guard; }

      const TInterface* operator->() const
      {
         return _guard;
      }

      TInterface* operator->()
      {
         return _guard;
      }

      operator const TInterface*() const
      {
         return _guard;
      }

      operator TInterface*()
      {
         return _guard;
      }

   private:
      HMODULE _handle = nullptr;
      ReleaseGuard<TInterface> _guard = nullptr;
   };
}
