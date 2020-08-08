#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>
#include "navdisp/NavDispTypes.h"
#include "DataStandartTypes.h"
#include "colreg/BaseRef.h"
#include "colreg/XmlProperties/xml_property_container.h"

namespace data_standart
{
   struct iDataStandart : colreg::iReleasable
   {
      virtual ~iDataStandart() = 0 { }

      virtual DataStandartType GetType() const = 0;

      virtual bool Create() = 0;

      virtual LPCSTR GetPath() = 0;

      virtual bool DeserializeAttrs(const xml_properties::PropertyItem&) = 0;
   };

   typedef std::shared_ptr<iDataStandart> iDataStandartPtr;
}