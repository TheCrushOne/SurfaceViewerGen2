/*!
\file
\brief Заголовочный файл property_helper.h
*/
#pragma once
#include "PropertyModify.h"
#include "colreg/PropertyStructs.h"

#define PROPHELPER_CREATEHOLDER_L(iPropPtr, name, description, prStruct, obj, field, callback) iPropPtr = std::make_unique<ValuePropertyHolder<prStruct, decltype(field)>>(name, description, false, VALUE_FORMAT_TYPE::VFT_NONE, obj, &prStruct::field, callback, this)
#define PROPHELPER_CREATEHOLDER(iPropPtr, prStruct, fieldType, obj, field, meta, sifi, callback) iPropPtr = std::make_unique<ValuePropertyHolder<prStruct, fieldType>>(meta.at(sifi).name.c_str(), meta.at(sifi).description.c_str(), meta.at(sifi).readOnly, meta.at(sifi).type, &obj, &prStruct::field, callback, this)
#define PROPHELPER_CREATEHOLDER_S(iPropPtr, prStruct, obj, field, meta, sifi, callback) PROPHELPER_CREATEHOLDER(iPropPtr, prStruct, decltype(obj.field), obj, field, meta, sifi, callback)

namespace SV
{
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

}