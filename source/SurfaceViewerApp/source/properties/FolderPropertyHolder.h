#pragma once

#include "PropertyModifyImpl.h"

namespace SV
{
   class FolderProperty
      : public Property
   {
   public:
      FolderProperty(const char* pname)
         : Property{ FieldMeta{ pname, "", VALUE_FORMAT_TYPE::VFT_NONE, true } }
      {}
      FolderProperty()
         : Property{ FieldMeta{ "General", "", VALUE_FORMAT_TYPE::VFT_NONE, true } }
      {}
      const char* get_value() const override { return ""; }
      void set_value(const char* c) override {}

      const prop_ref get_childs() const override
      {
         //return prop_ref(reinterpret_cast<const iPropertyInterface*>(_childs.data()), _childs.size());
         return prop_ref((iPropertyInterface**)_childs.data(), _childs.size());
      }
      const VectorProperties* get_child_list() const override
      {
         return &_childs;
      }
      PROPERTY_TYPE get_type() const override { return PROPERTY_TYPE::PT_FOLRDER; }
      void AddChild(iProperty* child)
      {
         _childs.push_back(child);
      }
   private:
      VectorProperties _childs;
   };
}