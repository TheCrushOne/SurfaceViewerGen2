#pragma once
//#include "CommonStructs.h"
//#include "CommonRefs.h"
#include "colreg/BaseRef.h"

#pragma pack (push, 1)

struct iPropertyInterface;
using prop_ref = colreg::base_ref<iPropertyInterface*>;

struct iPropertyInterface abstract
{
   virtual const char* get_name()const = 0;
   virtual const char* get_description()const = 0;
   virtual const char* get_value()const = 0;

   virtual void set_value(const char*) = 0;

   virtual const prop_ref get_childs()const = 0;
};

#pragma pack (pop)