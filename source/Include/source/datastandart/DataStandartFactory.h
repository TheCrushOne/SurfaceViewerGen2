#pragma once

#include "DataStandart.h"

namespace data_standart
{
   struct iDataStandartFactory
   {
      virtual iDataStandart* CreateDataStandart(DataStandartType type, LPCSTR name) = 0;
      virtual void DeleteDataStandart(LPCSTR name) = 0;

      virtual iDataStandart* GetDataStandart(LPCSTR name) const = 0;

      virtual void Clear() = 0;
   };
}