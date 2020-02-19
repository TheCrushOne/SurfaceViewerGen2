#pragma once
#include "TinyFixedStack.h"
#include <array>
#include <stdio.h>

//#define DEBUG_ALLOC_DEALLOC
#ifdef DEBUG_ALLOC_DEALLOC

#if _MSC_VER >= 1400

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

inline HMODULE GetCurrentModuleHandle()
{
   return (HMODULE)&__ImageBase;
}
#endif
#endif
namespace colreg
{
   namespace memory
   {
      class MemoryHolder
      {
         friend class MemoryHolderManager;

      private:

         MemoryHolder(size_t objSize, size_t objCount) :_objSize(objSize), _objCount(objCount), _freeObjects(objCount)
         {
#ifdef DEBUG_ALLOC_DEALLOC
            if (IsDebuggerPresent())
            {
               char buff[512];
               sprintf_s(buff, "Alloc Module %08X, MemoryHolder %08X\r\n", GetCurrentModuleHandle(), this);
               OutputDebugStringA(buff);
            }
#endif
            const auto align = 0xF;
            _rawBuffer = (char*)std::malloc(objSize * objCount + align);
            _memoryBuffer = (char*)(((uintptr_t)((char*)_rawBuffer + align)) & ~(uintptr_t)align);

            for (size_t index = 0; index < objCount; ++index)
            {
               _freeObjects.try_push_back(_memoryBuffer + index * _objSize);
            }
         

         }
         
         bool isClean() const 
         {
            return _freeObjects.get_free_size() == _objCount;
         }

         inline
         bool isOwner(void* addr) const { return addr >= _memoryBuffer && addr < (_memoryBuffer + _objSize * _objCount); }

         void* Alloc(size_t totalSize)
         {
            void* retVal = nullptr;
            if (_freeObjects.try_pop(retVal))
            {
               return retVal;
            }
            
            return nullptr;
         }

         void Dealloc(void* Addr, bool check = true )
         {
            if (check)
            {
               if (isOwner(Addr))
               {
                  _freeObjects.try_push_back(Addr);;
               }
               else
               {
                  throw std::bad_alloc();
               }

            }
            else
            {
               _freeObjects.try_push_back(Addr);
            }
         }

         ~MemoryHolder()
         {
#ifdef DEBUG_ALLOC_DEALLOC
            if (IsDebuggerPresent())
            {
               char buff[512];
               sprintf_s(buff, "Destroy Module %08X, MemoryHolder %08X\r\n", GetCurrentModuleHandle(), this);
               OutputDebugStringA(buff);
            }
#endif
            std::free(_rawBuffer);
         }

      private:
         //   std::vector< size_t >   _freeObjects;

         char*    _memoryBuffer = nullptr; //aligned buffer
         char*    _rawBuffer = nullptr;
         size_t   _objSize;
         size_t   _objCount;

         TinyFixedStack<void*> _freeObjects;
      };

      class MemoryHolderManager
      {
      public:
         MemoryHolderManager(size_t maxSize, size_t memoryBlockSize) :_maxSize(maxSize), _memBlock(memoryBlockSize)
         {
#ifdef DEBUG_ALLOC_DEALLOC
            if (IsDebuggerPresent())
            {
               char path[MAX_PATH];
               HMODULE hm = GetCurrentModuleHandle();

               char moduleName[528];
               GetModuleFileName(hm, moduleName, sizeof(moduleName));
               char buff[512];
               sprintf_s(buff, "Alloc Module %08X, file = %s, MemoryHolderManager %08X\r\n", hm, moduleName, this);
               OutputDebugStringA(buff);
            }
#endif
            for (auto& mh : _memHolders)
            {
               mh = nullptr;
            }
         }

         ~MemoryHolderManager()
         {
#ifdef DEBUG_ALLOC_DEALLOC
            if (IsDebuggerPresent())
            {
               char buff[512];
               sprintf_s(buff, "Destroy Module %08X, MemoryHolderManager %08X\r\n", GetCurrentModuleHandle(), this);
               OutputDebugStringA(buff);
            }
#endif

            for (auto*& memHolder : _memHolders)
            {
               if (memHolder)
               {
                  memHolder->~MemoryHolder();
                  std::free(memHolder);
                  memHolder = nullptr;
               }
            }
            if (_nextManager)
            {
               _nextManager->~MemoryHolderManager();
               std::free(_nextManager);
               _nextManager = nullptr;
            }
         }

         //should be called at dll main
         static void Enable(bool enabled)
         {
            _enabled = enabled;
            if (_enabled &&!_root)
            {
               _root = (MemoryHolderManager*)std::malloc(sizeof(MemoryHolderManager));
               new (_root) MemoryHolderManager(16384, 1024 * 1024 * 16);
            }

            if (!_enabled && _root)
            {
               if (_root->cleanup())
               {
                  destroy(_root);
                  _root = nullptr; //actually, _root clean inside  destroy
               }
            }
            
         }

         static bool IsEnabled() { return _enabled; }

         static void* Alloc(size_t totalSize)
         {
            if (_enabled && _root)
            {
               return _root->alloc(totalSize);
            }
            return std::malloc(totalSize);
         }

         
         static void Dealloc(void* Addr, size_t totalSize)
         {
            if (_root)
            {
               _root->dealloc(Addr, totalSize);
            }
            else
            {
               std::free(Addr);
            }
         }

         static void Dealloc(void* Addr)
         {
            if (_root)
            {
               _root->dealloc(Addr);
            }
            else
            {
               std::free(Addr);
            }
         }
        
      private:
         void* alloc(size_t totalSize)
         {
            if (!_enabled || totalSize > _maxSize)
            {
               return std::malloc(totalSize);
            }

            auto retVal = getMemoryHolder(getChunkIndex(totalSize))->Alloc(totalSize);
            if (retVal)
            {
               return retVal;
            }
            else
            {
               if (!_nextManager)
               {
                  _nextManager = (MemoryHolderManager*)std::malloc(sizeof(MemoryHolderManager));
                  ::new (_nextManager) MemoryHolderManager(_maxSize, _memBlock * 2);
               }
               return _nextManager->alloc(totalSize);
            }
         }

         //returns true if it destroys itself
         bool dealloc(void* Addr)
         {
            for (auto& mh : _memHolders)
            {
               if (mh && mh->isOwner(Addr))
               {
                  mh->Dealloc(Addr, false /* no check for owner */ );
                  return postDealloc();
               }
            }
            if (_nextManager)
            {
               if (_nextManager->dealloc(Addr))
               {
                  _nextManager = nullptr;
               }
            }
            else
            {
               std::free(Addr);
            }
            return false;
         }

         bool dealloc(void* Addr, size_t totalSize, size_t chunkIndex = 0)
         {
            if (totalSize > _maxSize)
            {
               std::free(Addr);
               return false;
            }
            if (totalSize <= sizeof(void*))
            {
               return dealloc(Addr);
            }
            
            if (!chunkIndex)
            {
               chunkIndex = getChunkIndex(totalSize);
            }
            auto mh = getMemoryHolder(chunkIndex);
            if (mh && mh->isOwner(Addr))
            {
               mh->Dealloc(Addr, false /* no check for owner*/);
               return postDealloc();
            }
            else
            {
               if (_nextManager)
               {
                  if (_nextManager->dealloc(Addr, totalSize, chunkIndex))
                  {
                     _nextManager = nullptr;
                  }
               }
               else
               {
                  std::free(Addr);
               }
            }
            return false;
         }

         //return true if all memory cleared
         bool cleanup()
         {
            bool clean = true;
            for (auto*& memHolder : _memHolders)
            {
               if (memHolder)
               {
                  if (memHolder->isClean())
                  {
                     memHolder->~MemoryHolder();
                     std::free(memHolder);
                     memHolder = nullptr;
                  }
                  else
                  {
                     clean = false;
                  }
               }
            }

            if (_nextManager)
            {
               
               if(_nextManager->cleanup() )
               { 
                  _nextManager->~MemoryHolderManager();
                  std::free(_nextManager);
                  _nextManager = nullptr;
               }
               else
               {
                  clean = false;
               }
               
            }

            return clean;
         }

         

         static void destroy(MemoryHolderManager* temp)
         {
            //destroying
            if (_root == temp)
            {
               _root = nullptr;
            }
            temp->~MemoryHolderManager();
            std::free(temp);
         }

         bool postDealloc()
         {
            if (!_enabled && cleanup())
            {
               destroy(this);
               return true;
            }
            return false;
         }

         MemoryHolder* getMemoryHolder(size_t chunkIndex)
         {
            if (_memHolders[chunkIndex])
            {
               return _memHolders[chunkIndex];
            }

            if (!_enabled)
            {
               return nullptr;
            }
            auto chunkSize = getChunkSizeByIndex(chunkIndex);
            auto mh = (MemoryHolder*)std::malloc(sizeof(MemoryHolder));
            ::new (mh)  MemoryHolder(chunkSize, _memBlock / chunkSize);
            _memHolders[chunkIndex] = mh;

            return mh;
         }

         size_t getChunkSizeByIndex(size_t index)
         {
            return 1 << (minSizeBits + index);
         }

         size_t getChunkIndex(size_t size) const
         {
            size_t ret = 0;
            constexpr size_t startChunk = 1 << minSizeBits;
            auto chunk = startChunk;
            while (chunk < size) { chunk <<= 1; ++ret; };
            return ret;
         }

      private:
         size_t _maxSize;
         size_t _memBlock;
         std::array< MemoryHolder*, 64 > _memHolders;
         static const unsigned minSizeBits = 4;//16 bytes
         MemoryHolderManager* _nextManager = nullptr;
         static bool _enabled ;
         static MemoryHolderManager* _root ;
      };
      __declspec(selectany) bool MemoryHolderManager::_enabled = false;
      __declspec(selectany) MemoryHolderManager* MemoryHolderManager::_root = nullptr;
     
   }
}