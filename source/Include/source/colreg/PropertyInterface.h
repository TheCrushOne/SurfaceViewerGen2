#pragma once
#include "CommonStructs.h"
#include "CommonRefs.h"

#pragma pack (push, 1)

namespace SV
{
   struct iPropertyInterface;
   typedef std::vector<iPropertyInterface*> prop_interface_vct;

   struct iPropertyInterface abstract
   {
      virtual const char* get_name() const = 0;
      virtual const char* get_description() const = 0;
      virtual const char* get_value() const = 0;

      virtual void set_value(const char*) = 0;

      virtual const prop_interface_vct* get_childs() const = 0;
   };
}
#pragma pack (pop)