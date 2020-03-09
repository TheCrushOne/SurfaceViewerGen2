#pragma once

namespace colreg
{
#pragma pack (push, 1)

   struct iReleasable
   {
      virtual void Release() = 0;

   protected:
      virtual ~iReleasable() = default;
   };

   struct ReleasableDeleter
   {
      void operator()(iReleasable* p)
      {
         if (p)
         {
            p->Release();
         }
      }
   };
   
   static auto interfaceDeleter = [](auto* releasableInterface) { releasableInterface->Release(); };
  

   /*
   \brief Базовая структура для передачи векторов в интерфейсах
   Создавать можно через BaseRefHolder
   */
   template<typename T>
   struct base_ref
   {
      base_ref() = default;

      base_ref(const T* p, size_t sz)
         : arr{ p }
         , size{ sz }
      {}

      const T* arr = nullptr;
      size_t size = 0;

      const T* begin()const{ return arr; }
      const T* end()const  { return arr + size; }
      const T& back()const { return arr[size-1]; }
      const T& operator[](size_t index) const { return arr[index]; }
   };

   /*
   \brief Базовый интерфейс для возвращения векторов в интерфейсах
   Освобождение ресурсов возлагается на клиента
   Создавать можно через BaseRefHolderReleasable
   */
   template<typename T>
   struct iBaseRefReleasable : public iReleasable
   {
      virtual const base_ref<T>* Get() const = 0;
   };

#pragma pack (pop)
} //namespace colreg