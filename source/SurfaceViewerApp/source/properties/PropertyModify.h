#pragma once

#include "property_structs.h"
#include "colreg/PropertyInterface.h"
#include <functional>

namespace SV
{
   struct iPropertyModify;
   using VectorProperties = std::vector<iPropertyModify*>;

   struct iPropertyModify abstract : iPropertyInterface
   {
      virtual ~iPropertyModify() = default;
      virtual PROPERTY_TYPE get_type()const = 0;
      virtual VALUE_FORMAT_TYPE get_value_format_type()const = 0;
      virtual bool is_read_only()const = 0;

      //virtual const VectorProperties* get_child_list()const = 0;
      virtual void set_list(const std::vector<std::string>& list) = 0;
      virtual const std::vector<std::string>& get_list() const = 0;
   };

   using iPropertyModifyPtr = std::unique_ptr<iPropertyModify>;
   typedef iPropertyModify* (*iPropModifyGetter)();
}