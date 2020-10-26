#pragma once

#include "colreg/PropertyInterface.h"
#include <functional>

namespace SV
{
   struct iPropertyModify;
   using VectorProperties = std::vector<iPropertyModify*>;

   struct iPropertyModify abstract : iPropertyInterface
   {
      virtual ~iPropertyModify() = default;
      //virtual const VectorProperties* get_child_list()const = 0;
      virtual void set_list(const std::vector<std::string>& list) = 0;
   };

   using iPropertyModifyPtr = std::unique_ptr<iPropertyModify>;
   typedef iPropertyModify* (*iPropModifyGetter)();
}