#pragma once

#include "colreg/PropertyInterface.h"
#include "colreg/PropertyStructs.h"
#include <functional>

namespace SV
{
   struct iProperty;
   using VectorProperties = std::vector<iProperty*>;

   struct iProperty abstract
      : iPropertyInterface
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

   class Property
      : public iProperty
   {
   public:
      Property(const FieldMeta& meta)
         : m_meta{ meta }
      {}

      const char* get_name() const override { return m_meta.name.c_str(); }
      const char* get_description()const override { return m_meta.description.c_str(); }
      VALUE_FORMAT_TYPE get_value_format_type() const override { return m_meta.type; }
      bool is_read_only()const override { return m_meta.readOnly; }
      const prop_ref get_childs()const override { return prop_ref(); }
      const VectorProperties* get_child_list()const override { return nullptr; };
      void set_list(const std::vector<std::string>& list) override { _list = list; }
      const std::vector<std::string>& get_list() const { return _list; }
   protected:
      FieldMeta m_meta;
      std::vector<std::string> _list;
   };
}