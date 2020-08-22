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
      bool needToRewrite(size_t hash) const
      {
         std::ifstream file(getHashPath(), std::ios::in | std::ios::binary);
         char* buffer = new char[sizeof(double)];
         file.read(buffer);
         size_t f_hash = *reinterpret_cast<size_t*>buffer;
         file.close();
         return f_hash != hash;
      }
      virtual LPCSTR getPath() = 0;
      // Common
      std::string getHashPath() { return std::string(getPath()) + "\\control.hash"; }
      void saveHash(size_t hash)
      {
         std::ofstream file(getHashPath(), std::ios::out | std::ios::binary);
         file.write(hash);
         file.close();
      }
   protected:
      std::wstring m_baseFolder;
      navigation_dispatcher::iComService* m_service;
      TDataStandartData m_dataStandartData;
   };
}