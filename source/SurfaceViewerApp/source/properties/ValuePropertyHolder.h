#pragma once

#include "PropertyModifyImpl.h"
#include "property_helper.h"

namespace SV
{
   template< class T, typename Type >
   class ValuePropertyHolder
      : public Property
      , private xml_properties::FormatType
   {
      typedef Type(T::* pValue);
      using CallBackFunctor = std::function<void()>;

   public:
      template<class CallBack, class... Args>
      ValuePropertyHolder(const FieldMeta& meta, T* holder, pValue pvalue, CallBack async_task, Args... args)
         : Property(meta)
         , _holder(holder)
         , _pvalue(pvalue)
      {
         _callBackFunctor = std::bind(async_task, args...);
      }

      ~ValuePropertyHolder() {}

      const char* get_value() const override
      {
         std::stringstream buffer;
         buffer.setf(std::ios_base::boolalpha | std::ios_base::fixed);

         if constexpr (std::is_arithmetic<Type>::value != 0)
         {
            _value = m_meta.type != VALUE_FORMAT_TYPE::VFT_NONE ? get_formated_value(_holder->*_pvalue, m_meta.type) :
               xml_properties::FormatType::getFormatedValue(_holder->*_pvalue);
         }
         else
            _value = xml_properties::FormatType::getFormatedValue(_holder->*_pvalue);

         return _value.c_str();
      }

      void set_value(const char* value) override
      {
         //       if (_value == value)
         //       {
         //          return;
         //       }
         (_holder->*_pvalue) = xml_properties::FormatType::stringToValue<Type>(value);
         if (_callBackFunctor)
            _callBackFunctor();
      }

      PROPERTY_TYPE get_type() const override
      {
         if constexpr (std::is_enum<Type>::value != 0)
            return PROPERTY_TYPE::PT_TEXT_LIST;
         else
            return _list.empty() ? getType<Type>() : PROPERTY_TYPE::PT_TEXT_LIST;
      }
   private:
      template< class Type >
      PROPERTY_TYPE getType() const { return PROPERTY_TYPE::PT_VALUE; }

      template <>
      PROPERTY_TYPE getType<bool>()const { return PROPERTY_TYPE::PT_BOOL; }

      template <>
      PROPERTY_TYPE getType<COLORREF>()const { return PROPERTY_TYPE::PT_COLOR; }

   private:
      T* _holder;
      pValue				_pvalue = nullptr;
      CallBackFunctor   _callBackFunctor = nullptr;
      mutable std::string       _value;
   };
}