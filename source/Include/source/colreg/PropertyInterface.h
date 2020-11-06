#pragma once
#include "CommonStructs.h"
#include "CommonRefs.h"
#include "PropertyStructs.h"
#include "colreg/BaseRef.h"

//#pragma pack (push, 1)

namespace SV
{
   struct iPropertyInterface;
   using prop_ref = base_ref<iPropertyInterface*>;

   struct iPropertyInterface abstract
   {
      virtual const char* get_name()const = 0;
      virtual const char* get_description()const = 0;
      virtual const char* get_value()const = 0;

      virtual void set_value(const char*) = 0;

      virtual const prop_ref get_childs()const = 0;
   };

   //typedef std::shared_ptr<iPropertyInterface> iPropertyInterfacePtr;
}
//#pragma pack (pop)