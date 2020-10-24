#pragma once

#include "PropertyModifyImpl.h"

namespace SV
{
   class FolderPropertyHolder : public PropertyModifyImpl
   {
   public:
      FolderPropertyHolder(const char* pname)
         : PropertyModifyImpl{ pname, "", true, VALUE_FORMAT_TYPE::VFT_NONE }
      {}
      FolderPropertyHolder()
         : PropertyModifyImpl{ "General", "", true, VALUE_FORMAT_TYPE::VFT_NONE }
      {}
      const char* get_value()const override { return ""; }
      void set_value(const char* c)override {}

      // TODO: fix!!!
      const prop_interface_vct* get_childs() const override final { return nullptr; }
      //const simple_prop_vct get_childs()const override
      //{
         //return prop_ref(reinterpret_cast<const iPropertyInterface*>(_childs.data()), _childs.size());
         //return prop_ref((iPropertyInterface**)_childs.data(), _childs.size());
      //}
      /*const VectorProperties* get_child_list()const override
      {
         return &_childs;
      }*/
      PROPERTY_TYPE get_type()const override { return PROPERTY_TYPE::PT_FOLRDER; }
      void AddChild(iProperty* child)
      {
         _childs.push_back(child);
      }
   private:
      VectorProperties _childs;
   };
}