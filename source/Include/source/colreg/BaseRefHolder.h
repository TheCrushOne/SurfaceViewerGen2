#pragma once

#include "BaseRef.h"

////! Базовая структура для хранения векторов при возврате из интерфейса
//template<typename T>
//class BaseRefHolder : public colreg::base_ref<T>
//{
//public:
//   BaseRefHolder() = default;
//
//   BaseRefHolder(const colreg::base_ref<T>* r)
//      : colreg::base_ref<T>(r->arr, r->size)
//      , _vec(r->arr, r->arr + r->size)
//   {}
//
//   BaseRefHolder(const std::vector<T>& vec)
//      : _vec(vec)
//   {
//      Rebase();
//   }
//
//   BaseRefHolder(std::vector<T>&& vec)
//      : _vec(std::move(vec))
//   {
//      Rebase();
//   }
//
//   BaseRefHolder(const BaseRefHolder& other)
//      : _vec{ other._vec }
//   {
//      Rebase();
//   }
//   
//   BaseRefHolder(BaseRefHolder&& other) = default;
//
//   BaseRefHolder& operator = (const BaseRefHolder& other)
//   {
//      if (this != &other)
//      {
//         _vec = other._vec;
//         Rebase();
//      }
//      return *this;
//   }
//
//   BaseRefHolder& operator = (BaseRefHolder&& other) = default;
//
//   inline void Resize(size_t new_size)
//   {
//      _vec.resize(new_size);
//      colreg::base_ref<T>::size = new_size;
//      colreg::base_ref<T>::arr = _vec.data();
//   }
//
//   inline T& operator[](size_t index) { return _vec[index]; }
//   inline std::vector<T>& Data() { return _vec; }
//   inline const std::vector<T>& Data()const { return _vec; }
//
//   inline void Rebase()
//   {
//      colreg::base_ref<T>::size = _vec.size();
//      colreg::base_ref<T>::arr = _vec.data();
//   }
//
//private:
//   std::vector<T> _vec;
//};
//
//template<typename T>
//struct BaseRefHolderReleasable 
//   : public BaseRefHolder<T>
//   , public colreg::iBaseRefReleasable<T>
//{
//public:
//   BaseRefHolderReleasable() = default;
//
//   BaseRefHolderReleasable(std::vector<T>&& vec)
//      : BaseRefHolder(std::move(vec))
//   {}
//
//   void Release() final { delete this; }
//
//   const colreg::base_ref<T>* Get() const final { return this; }
//};