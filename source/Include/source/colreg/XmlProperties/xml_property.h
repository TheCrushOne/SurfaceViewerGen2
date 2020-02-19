#pragma once
#include <unordered_map>
#include <sstream>
#include <iomanip>

namespace xml_properties
{
   struct FormatType
   {
      static std::stringstream setupBuffer()
      {
         std::stringstream buffer;
         buffer.setf(std::ios_base::boolalpha | std::ios_base::fixed);

         return buffer;
      };

      template< class Type >
      inline static std::string getFormatedValue(const Type& value)
      {
         auto buffer = setupBuffer();
         if constexpr (std::is_enum<Type>::value != 0)
            buffer << (size_t)value;
         else
            buffer << value;

         return buffer.str();
      }

      template <>
      inline static std::string getFormatedValue<double>(const double& value)
      {
         auto buffer = setupBuffer();

         buffer << std::setprecision(std::numeric_limits<double>::digits10) << value;

         return buffer.str();
      }

      template <>
      inline static std::string getFormatedValue<bool>(const bool& value)
      {
         auto buffer = setupBuffer();

         buffer << (value ? 1 : 0);

         return buffer.str();
      }

      template <>
      inline static std::string getFormatedValue<uint8_t>(const uint8_t& value) { return std::to_string(value); }

      template< class Type >
      inline static Type stringToValue(const char* pValue)
      {
         auto buffer { setupBuffer() };
         Type temp;
         buffer << pValue;
         if constexpr (std::is_enum<Type>::value != 0)
         {
            size_t i;
            buffer >> i;
            temp = (Type)i;
         }
         else
            buffer >> temp;
         
         return temp;
      }

      template<>
      inline static std::string stringToValue(const char* pValue)
      {
         auto buffer{ setupBuffer() };
         std::string temp;
         buffer << pValue;
         return buffer.str();
      }

      template<>
      inline static bool stringToValue<bool>(const char* pValue)
      {
         auto buffer{ setupBuffer() };
         int temp;

         buffer << pValue;
         buffer >> temp;

         return (temp != 0);
      }

      template<>
      inline static const void* stringToValue<const void*>(const char* pValue) { return reinterpret_cast<const void*>(pValue); }
   };


   class ValueProperty : private FormatType
   {
   public:
      ValueProperty() = default;
      ValueProperty(const char* pname, const char* pdesc) : _name(pname), _desc(pdesc) {  }
   public:

      template<typename Type>
      Type GetValue() const
      {
         return stringToValue<Type>(_value.c_str());
      }
      template<typename Type>
      void SetValue(Type cvalue)
      {
         _value = getFormatedValue<Type>(cvalue);
      }


      const char* GetCharValue() const
      {
         return _value.c_str();
      }
      void SetCharValue(const char* pvalue)
      {
         _value = pvalue;
      }


      template<typename Type>
      Type GetSubValue() const
      {
         return stringToValue<Type>(_subvalue.c_str());
      }
      template<typename Type>
      void SetSubValue(Type cvalue)
      {
         _subvalue = getFormatedValue<Type>(cvalue);
      }


      const char* GetCharSubValue() const
      {
         return _subvalue.c_str();
      }
      void SetCharSubValue(const char* pvalue)
      {
         _subvalue = pvalue;
      }


      const char* GetDesc() const
      {
         return _desc.c_str();
      }
      void SetDesc(const char* cdesc)
      {
         _desc = std::string(cdesc);
      }


      const char* GetName() const
      {
         return _name.c_str();
      }
      void SetName(const char* cname)
      {
         _name = std::string(cname);
      }


      bool IsEmpty() const
      {
         return _value.empty() && _name.empty();
      }
   private:
      std::string  _name;
      std::string  _desc;
      std::string  _value;
      std::string  _subvalue;
   };
}



