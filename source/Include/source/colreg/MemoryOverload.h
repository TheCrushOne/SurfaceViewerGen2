#pragma once

#include <cstdio>
#include <new>
#include "MemoryBlock.h"

void* operator new  (std::size_t count);
void* operator new[](std::size_t count);


void* operator new  (std::size_t count, std::nothrow_t const&)  noexcept;
void* operator new[](std::size_t count, const std::nothrow_t& tag) noexcept;

void operator delete  (void* ptr);
void operator delete[](void* ptr);

void operator delete  (void* ptr, std::size_t sz);
void operator delete[](void* ptr, std::size_t sz);

void operator delete  (void* ptr, size_t al, const std::nothrow_t& tag);
void operator delete[](void* ptr, size_t al, const std::nothrow_t& tag);

void operator delete  (void* ptr, const std::nothrow_t& tag);
void operator delete[](void* ptr, const std::nothrow_t& tag);