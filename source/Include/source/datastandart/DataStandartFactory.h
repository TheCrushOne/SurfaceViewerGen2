#pragma once

#include "DataStandart.h"

namespace data_standart
{
   struct iDataStandartFactory
   {
      virtual iDataStandart* CreateDataStandart(DataStandartType type, LPCWSTR name) = 0;
      virtual void DeleteDataStandart(LPCWSTR name) = 0;

      virtual iDataStandart* GetDataStandart(LPCWSTR name) const = 0;

      virtual void Clear() = 0;
   };
}