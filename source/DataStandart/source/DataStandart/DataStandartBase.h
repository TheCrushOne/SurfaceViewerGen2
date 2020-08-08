#pragma once

#include "datastandart\DataStandart.h"

namespace data_standart
{
   template <int TDataStandartType, typename TDataStandartData>
   class DataStandart : public Central, public iDataStandart
   {
      enum { data_standart_type = TDataStandartType };
   protected:
      DataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
         : Central(pack)
         , m_baseFolder(base_folder)
         , m_service(pService)
      {}
      DataStandartType GetType() const override
      {
         return static_cast<DataStandartType>(data_standart_type);
      }
      bool Create() override
      {
         if (!m_dataStandartData.clear_dst)
         {
            // try to open
            if (open())
               return true; // Database is already exist. Do not create, just open.
         }
         return create();
      }
      LPCSTR GetPath() override { return getPath(); }
      void Release() override { delete this; }
      bool DeserializeAttrs(const xml_properties::PropertyItem& standart) override final { return m_dataStandartData.Deserialize(standart); }
   protected:
      virtual LPCSTR getPath() = 0;
      virtual bool create() = 0;
      virtual bool open() = 0;
   protected:
      std::wstring m_baseFolder;
      navigation_dispatcher::iComService* m_service;
      TDataStandartData m_dataStandartData;
   };
}