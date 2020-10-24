/*!
\file
\brief Заголовочный файл property_helper.h
*/
#pragma once
#include "property.h"

#define PROPHELPER_CREATEHOLDER_L(iPropPtr, name, description, prStruct, obj, field, callback) iPropPtr = std::make_unique<ValuePropertyHolder<prStruct, decltype(field)>>(name, description, false, VALUE_FORMAT_TYPE::VFT_NONE, obj, &prStruct::field, callback, this)
#define PROPHELPER_CREATEHOLDER(iPropPtr, prStruct, fieldType, obj, field, meta, sifi, callback) iPropPtr = std::make_unique<ValuePropertyHolder<prStruct, fieldType>>(meta.at(sifi).name.c_str(), meta.at(sifi).description.c_str(), meta.at(sifi).readOnly, meta.at(sifi).type, &obj, &prStruct::field, callback, this)
#define PROPHELPER_CREATEHOLDER_S(iPropPtr, prStruct, obj, field, meta, sifi, callback) PROPHELPER_CREATEHOLDER(iPropPtr, prStruct, decltype(obj.field), obj, field, meta, sifi, callback)
namespace SV
{
   struct FieldMeta
   {
      std::string name;
      std::string description;
      VALUE_FORMAT_TYPE type;
      bool readOnly;
   };
}