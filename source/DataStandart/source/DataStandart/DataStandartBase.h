#pragma once

#include "datastandart\DataStandart.h"

namespace data_standart
{
   template <int TDataStandartType, typename TDataStandartData>
   class DataStandart : public Central
   {
      enum { data_standart_type = TDataStandartType };
   protected:
      DataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
         : Central(pack)
         , m_baseFolder(base_folder)
         , m_service(pService)
      {}

   protected:
      bool deserializeAttrs(const xml_properties::PropertyItem& standart) { return m_dataStandartData.Deserialize(standart); }
      DataStandartType getType() const { return static_cast<DataStandartType>(data_standart_type); }
      virtual LPCSTR getPath() = 0;
   protected:
      std::wstring m_baseFolder;
      navigation_dispatcher::iComService* m_service;
      TDataStandartData m_dataStandartData;
   };
}