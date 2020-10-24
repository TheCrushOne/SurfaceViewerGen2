/*!
\file
\brief Заголовочный файл property.h
*/
#pragma once
#include "colreg\XmlProperties\xml_property.h"
#include "colreg/PropertyInterface.h"
#include "common/properties.h"
#include <functional>

namespace SV
{
   enum class PROPERTY_TYPE
   {
      PT_VALUE = 0,
      PT_BOOL,
      PT_TEXT,
      PT_TEXT_LIST,
      PT_FOLRDER
   };

   enum class VALUE_FORMAT_TYPE
   {
      VFT_NONE = 0,
      VFT_SPEED,
      VFT_SPEED_MS,
      VFT_COURSE,
      VFT_DISTANCE,
      VFT_DISTANCE_METERS,
      VFT_TIME_SEC,
      VFT_COORD_LAT,
      VFT_COORD_LON,
      VFT_PERCENT,

      VFT_LOGFILE,
      VFT_FOLDERPATH,
   };

   template<typename T>
   inline std::string get_formated_value(T value, VALUE_FORMAT_TYPE format_type)
   {
      std::stringstream buffer;
      if constexpr (std::is_same<T, bool>::value != 0)
         return "";
      else
      {
         buffer.setf(std::ios_base::boolalpha | std::ios_base::fixed | std::ios_base::showbase);
         buffer.fill('0');
         switch (format_type)
         {
         case VALUE_FORMAT_TYPE::VFT_NONE:
            break;
         case VALUE_FORMAT_TYPE::VFT_SPEED:
         {
            buffer.width(4);
            buffer << std::setprecision(1) << value << " kts";
            break;
         }
         case VALUE_FORMAT_TYPE::VFT_SPEED_MS:
         {
            buffer.width(4);
            buffer << std::setprecision(1) << value << " m/s";
            break;
         }
         case VALUE_FORMAT_TYPE::VFT_COURSE:
         {
            buffer.width(5);
            buffer << std::setprecision(1) << value << "°";
            break;
         }
         case VALUE_FORMAT_TYPE::VFT_DISTANCE:
         {
            buffer.width(5);
            buffer << std::setprecision(2) << value << " NM";
            break;
         }
         case VALUE_FORMAT_TYPE::VFT_DISTANCE_METERS:
         {
            buffer.width(3);
            buffer << (int)value << " m";
            break;
         }
         case VALUE_FORMAT_TYPE::VFT_TIME_SEC:
         {
            buffer.width(5);
            buffer << (int)value << " s";
            break;
         }
         case VALUE_FORMAT_TYPE::VFT_PERCENT:
         {
            buffer.width(3);
            buffer << (int)value << " %";
            break;
         }
         case VALUE_FORMAT_TYPE::VFT_COORD_LAT:
            (value < 0.) ? buffer << "-" : buffer << "+";
            buffer.width(7);
            buffer << std::setprecision(4) << value << "°";
            break;
         case VALUE_FORMAT_TYPE::VFT_COORD_LON:
            (value < 0.) ? buffer << "-" : buffer << "+";
            buffer.width(8);
            buffer << std::setprecision(4) << value << "°";
            break;
         case VALUE_FORMAT_TYPE::VFT_LOGFILE:
            buffer << value;
            break;
         case VALUE_FORMAT_TYPE::VFT_FOLDERPATH:
            buffer << value;
            break;
         }
      }
      return buffer.str();
   }

   struct iProperty;
   using VectorProperties = std::vector<iProperty*>;

   struct iProperty abstract : iPropertyInterface
   {
      virtual ~iProperty() = default;
      virtual PROPERTY_TYPE get_type()const = 0;
      virtual VALUE_FORMAT_TYPE get_value_format_type()const = 0;
      virtual bool is_read_only()const = 0;

      virtual const VectorProperties* get_child_list()const = 0;
      virtual void set_list(const std::vector<std::string>& list) = 0;
      virtual const std::vector<std::string>& get_list() const = 0;
   };

   using iPropertyPtr = std::unique_ptr< iProperty>;
   typedef iProperty* (*iPropGetter)();


   class Property : public iProperty
   {
   public:
      Property(const char* name, const char* description, bool readonly, VALUE_FORMAT_TYPE type)
         : _name{ name }
         , _description{ description }
         , _readonly{ readonly }
         , _format_type{ type }
      {}

      const char* get_name()const override { return _name.c_str(); }
      const char* get_description()const override { return _description.c_str(); }
      VALUE_FORMAT_TYPE get_value_format_type()const override { return _format_type; }
      bool is_read_only()const override { return _readonly; }
      //const properties::simple_prop_vct& get_childs()const override { return properties::simple_prop_vct{}; }
      const VectorProperties* get_child_list()const override { return nullptr; };
      void set_list(const std::vector<std::string>& list) override { _list = list; }
      const std::vector<std::string>& get_list() const { return _list; }
   protected:
      std::string _name;
      std::string _description;
      VALUE_FORMAT_TYPE _format_type;
      bool _readonly;
      std::vector<std::string> _list;
   };

   template< class T, typename Type >
   class ValuePropertyHolder : public Property, private xml_properties::FormatType
   {
      typedef Type(T::* pValue);
      using CallBackFunctor = std::function<void()>;

   public:
      template<class CallBack, class... Args>
      ValuePropertyHolder(const char* name, const char* description, bool readonly, VALUE_FORMAT_TYPE type, T* holder, pValue pvalue, CallBack async_task, Args... args) : Property(name, description, readonly, type), _holder(holder), _pvalue(pvalue)
      {
         _callBackFunctor = std::bind(async_task, args...);
      }

      ~ValuePropertyHolder() {}

      const char* get_value()const override
      {
         std::stringstream buffer;
         buffer.setf(std::ios_base::boolalpha | std::ios_base::fixed);

         if constexpr (std::is_arithmetic< Type>::value != 0)
         {
            _value = _format_type != VALUE_FORMAT_TYPE::VFT_NONE ? get_formated_value(_holder->*_pvalue, _format_type) :
               xml_properties::FormatType::getFormatedValue(_holder->*_pvalue);
         }
         else
            _value = xml_properties::FormatType::getFormatedValue(_holder->*_pvalue);

         return _value.c_str();
      }

      void set_value(const char* value)override
      {
         //       if (_value == value)
         //       {
         //          return;
         //       }
         (_holder->*_pvalue) = xml_properties::FormatType::stringToValue<Type>(value);
         if (_callBackFunctor)
            _callBackFunctor();
      }

      PROPERTY_TYPE get_type()const override
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


   private:
      T* _holder;
      pValue				_pvalue = nullptr;
      CallBackFunctor   _callBackFunctor = nullptr;
      mutable std::string       _value;
   };


   class FolderProperty : public Property
   {
   public:
      FolderProperty(const char* pname)
         : Property{ pname, "", true, VALUE_FORMAT_TYPE::VFT_NONE }
      {}
      FolderProperty()
         : Property{ "General", "", true, VALUE_FORMAT_TYPE::VFT_NONE }
      {}
      const char* get_value()const override { return ""; }
      void set_value(const char* c)override {}

      //const simple_prop_vct get_childs()const override
      //{
         //return prop_ref(reinterpret_cast<const iPropertyInterface*>(_childs.data()), _childs.size());
         //return prop_ref((iPropertyInterface**)_childs.data(), _childs.size());
      //}
      const VectorProperties* get_child_list()const override
      {
         return &_childs;
      }
      PROPERTY_TYPE get_type()const override { return PROPERTY_TYPE::PT_FOLRDER; }
      void AddChild(iProperty* child)
      {
         _childs.push_back(child);
      }
   private:
      VectorProperties _childs;
   };
}