#include "stdafx.h"

#define OVERLOAD_NEW_DELETE

#ifdef OVERLOAD_NEW_DELETE

#include "MemoryOverload.h"

void* operator new  (std::size_t count)
{
   return colreg::memory::MemoryHolderManager::Alloc(count);
}
void* operator new[](std::size_t count)
{
   return colreg::memory::MemoryHolderManager::Alloc(count);
}

void* operator new  (std::size_t count, const std::nothrow_t& tag) noexcept
{
   return colreg::memory::MemoryHolderManager::Alloc(count);
}

void* operator new[](std::size_t count, const std::nothrow_t& tag) noexcept
{
   return colreg::memory::MemoryHolderManager::Alloc(count);
}

void operator delete  (void* ptr)
{
   colreg::memory::MemoryHolderManager::Dealloc(ptr);
}

void operator delete[](void* ptr)
{
   colreg::memory::MemoryHolderManager::Dealloc(ptr);
}

void operator delete  (void* ptr, std::size_t sz)
{
   colreg::memory::MemoryHolderManager::Dealloc(ptr, sz);
}
void operator delete[](void* ptr, std::size_t sz)
{
   colreg::memory::MemoryHolderManager::Dealloc(ptr, sz);
}

void operator delete  (void* ptr, size_t al, const std::nothrow_t& tag)
{
   colreg::memory::MemoryHolderManager::Dealloc(ptr, al);
}

void operator delete[](void* ptr, size_t al, const std::nothrow_t& tag)
{
   colreg::memory::MemoryHolderManager::Dealloc(ptr, al);
}

void operator delete  (void* ptr, const std::nothrow_t& tag)
{
   colreg::memory::MemoryHolderManager::Dealloc(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t& tag)
{
   colreg::memory::MemoryHolderManager::Dealloc(ptr);
}
#endif