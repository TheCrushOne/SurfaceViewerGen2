#pragma once

template<class T>
class HolderBase
{
public:
   HolderBase(T* pInterface)
      : _pInterface(pInterface)
   {}

   inline T* Get()
   {
      return _pInterface;
   }

   inline const T* Get() const
   {
      return _pInterface;
   }

private:
   // forbidden
   HolderBase(const HolderBase&);
   void operator = (const HolderBase&);

protected:
   T* _pInterface;
};