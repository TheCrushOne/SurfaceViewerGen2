#pragma once
#include "CommonStructs.h"
#include "CommonRefs.h"
#include "PropertyStructs.h"

#pragma pack (push, 1)

namespace SV
{
   struct iPropertyInterface;
   typedef std::vector<iPropertyInterface*> prop_interface_vct;

   struct iPropertyInterface abstract
   {
      virtual VALUE_FORMAT_TYPE get_value_format_type()const = 0;
      virtual const char* get_name() const = 0;
      virtual PROPERTY_TYPE get_type() const = 0;
      virtual const char* get_description() const = 0;
      virtual const char* get_value() const = 0;
      virtual const std::vector<std::string>& get_list() const = 0;
      virtual bool is_read_only()const = 0;

      virtual void set_value(const char*) = 0;

      virtual const prop_interface_vct* get_childs() const = 0;
   };

   typedef std::shared_ptr<iPropertyInterface> iPropertyInterfacePtr;
}
#pragma pack (pop)