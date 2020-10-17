#pragma once

#include <typeindex>
#include <limits>
#include "colreg/CommonStructs.h"
#include "SVCG\base_geometry.h"

namespace colreg
{
   bool checkValue(const xml_properties::PropertyItem* item)
   {
      std::string data;
      item->Get(data);
      return data.length() == 0;
   }

   template<typename T>
   void setValue(xml_properties::PropertyItem& item, const T value)
   {
      item = value;
   }

   template<>
   void setValue(xml_properties::PropertyItem& item, const double value)
   {
      value != SVCG::NO_VALUE ? item = value : item = "NO_VALUE";
   }

   template<typename T>
   T getValue(const xml_properties::PropertyItem& item)
   {
      T value;
      item.Get(value);
      return value;
   }

   template<>
   double getValue(const xml_properties::PropertyItem& item)
   {
      std::string str;
      item.Get(str);
      return str.compare("NO_VALUE") ? atof(str.c_str()) : SVCG::NO_VALUE;
   }

   template<typename DataType = settings::pathfinding_settings>
   struct iSettingsTree
   {
      iSettingsTree(std::string tag, std::string desc, void* value, const void* defvalue, bool diff)
         : _tag(tag)
         , _desc(desc)
         , _value(value)
         , _defvalue(defvalue)
         , _diff(diff)
      {}

      iSettingsTree(std::string tag, std::string desc, std::vector<iSettingsTree<DataType>*> children)
         : _tag(tag)
         , _desc(desc)
         , _value(nullptr)
         , _defvalue(nullptr)
         , _children(children)
      {}
      operator std::vector<iSettingsTree>& ()
      {
         return _children;
      }

      virtual void SerializeValue(xml_properties::PropertyItem* item) = 0;
      virtual void DeserializeValue(const xml_properties::PropertyItem* item) const = 0;
      virtual bool CompareValues() const = 0;
      virtual ~iSettingsTree()
      {
         for (auto& child : _children)
         {
            if (child)
            {
               delete child;
               child = nullptr;
            }
         }
         _children;
      }

      std::string _tag;
      std::string _desc;

      std::vector<iSettingsTree<DataType>*> _children;

      void* _value;
      const void* _defvalue;
      bool _diff;
   };

   template<typename ValType = bool, typename DataType = settings::pathfinding_settings>
   struct TypedSettingsTree : public iSettingsTree<DataType>
   {
      TypedSettingsTree(std::string tag, std::string desc, ValType* value, const ValType* defvalue, bool diff, bool checker)
         : iSettingsTree<DataType>(tag, desc, value, defvalue, diff)
      {}

      TypedSettingsTree(std::string tag, std::string desc, std::vector<iSettingsTree<DataType>*> children)
         : iSettingsTree<DataType>(tag, desc, children)
      {}

      virtual bool CompareValues() const
      {
         return !std::is_same<ValType, void*>::value && iSettingsTree<DataType>::_diff && (*reinterpret_cast<ValType*>(iSettingsTree<DataType>::_value) == *reinterpret_cast<const ValType*>(iSettingsTree<DataType>::_defvalue));
      }

      virtual void SerializeValue(xml_properties::PropertyItem* item)
      {
         if (!CompareValues())
         {
            ATLASSERT(item);
            setValue(item, reinterpret_cast<ValType*>(iSettingsTree<DataType>::_value));
            item->SetDesc(iSettingsTree<DataType>::_desc.c_str());
         }
      }

      template<typename ValType>
      inline typename std::enable_if<!std::is_enum<ValType>::value, void>::type
         setValue(xml_properties::PropertyItem* item, ValType* value) const { *item = *value; }
      template<typename ValType>
      inline typename std::enable_if<std::is_enum<ValType>::value, void>::type
         setValue(xml_properties::PropertyItem* item, ValType* value) const { *item = static_cast<size_t>(*value); }
      template<>
      void setValue(xml_properties::PropertyItem* item, void* value) const {}

      virtual void DeserializeValue(const xml_properties::PropertyItem* item) const
      {
         ATLASSERT(item);
         getValue((xml_properties::PropertyItem*)item, reinterpret_cast<ValType*>(iSettingsTree<DataType>::_value));
      }

      template<typename ValType>
      inline typename std::enable_if<!std::is_enum<ValType>::value, void>::type
         getValue(xml_properties::PropertyItem* item, ValType* value) const
      {
         bool isEmpty = checkValue(item);
         if (!isEmpty)
            item->Get(*value);
      }
      template<typename ValType>
      inline typename std::enable_if<std::is_enum<ValType>::value, void>::type
         getValue(xml_properties::PropertyItem* item, ValType* value) const
      {
         size_t val;
         bool isEmpty = checkValue(item);
         if (!isEmpty)
         {
            item->Get(val);
            *value = static_cast<ValType>(val);
         }
      }
      template<>
      void getValue(xml_properties::PropertyItem* item, void* value) const {}
   };
}