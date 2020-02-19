#pragma once
#include <stdlib.h>
#include <exception>

template< typename T, typename indexType = int>
class TinyFixedStack
{
public:
   TinyFixedStack(size_t stackSize) :_stackSize(stackSize), _lastFreeIndex(0)
   {
      _stackData = (T*)std::malloc(sizeof(_stackData) * _stackSize);
   }

   ~TinyFixedStack()
   {
      if (_stackData)
      {
         std::free(_stackData);
         _stackData = nullptr;
      }
   }

   size_t get_free_size() const
   {
      return _lastFreeIndex ;
   }

   bool try_push_back(const T& val)
   {
      if ( static_cast<size_t>(_lastFreeIndex ) >= _stackSize )
      {
         return false;
      }
      _stackData[_lastFreeIndex++] = val;
      return true;
   }

   bool try_pop(T& val)
   {
      if (_lastFreeIndex > 0)
      {
         auto index = --_lastFreeIndex;
         if(index>=0)
         { 
            val = _stackData[index];
            return true;
         }
         ++_lastFreeIndex;
         return false;
      }
      return false;
   }

private:
   size_t   _stackSize;
   T*       _stackData;
   indexType   _lastFreeIndex;
};
