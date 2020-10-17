#pragma once

#include "NavDispTypes.h"
#include "colreg/BaseRef.h"
#include "colreg/XmlProperties/xml_property_container.h"
#include <memory>

namespace SV::navigation_dispatcher
{
   struct iOrder : iReleasable
   {
      virtual ~iOrder() = 0 {}

      virtual OrderType GetType() const = 0;

      //virtual iFilterProcessor* GetFilters() = 0;

      virtual bool IsMultithread() const = 0;

      virtual void SetThreadNum(unsigned int threadNum) = 0;

      virtual bool Process() = 0;

      virtual bool DeserializeAttrs(const xml_properties::PropertyItem&) = 0;
   };
}