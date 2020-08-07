#pragma once

#include "datastandart/DataStandartFactory.h"
#include "navdisp/ComService.h"
#include "colreg/ModuleGuard.h"
#include <comdef.h>
#include <map>

using StandartModuleGuard = colreg::ModuleGuard<data_standart::iDataStandart, central_pack*, LPCWSTR, navigation_dispatcher::iComService*>;

class DataStandartFactoryImpl
   : public data_standart::iDataStandartFactory, public Central
{
public:
   DataStandartFactoryImpl(central_pack* pack, navigation_dispatcher::iComService* pService)
      : Central(pack)
      , m_dataStandartImplInt(pack, pService)
   {
   }

   // iDataSourceFactory
private:
   virtual data_standart::iDataStandart* CreateDataStandart(data_standart::DataStandartType type, LPCWSTR name);
   virtual void DeleteDataStandart(LPCWSTR name);

   virtual data_standart::iDataStandart* GetDataStandart(LPCWSTR name) const;

   virtual LPCWSTR GetBaseFolder() const;
   virtual void SetBaseFolder(LPCWSTR baseFolder);

   virtual LPCWSTR GetScriptFolder() const;
   virtual void SetScriptFolder(LPCWSTR scriptFolder);

   virtual void Clear();

   class DataStandartFactoryImplInternal : public Central
   {
   public:
      DataStandartFactoryImplInternal(central_pack* pack, navigation_dispatcher::iComService* pService)
         : Central(pack)
         , m_services(pService)
      {
      }

      virtual data_standart::iDataStandart* CreateDataStandart(data_standart::DataStandartType type, LPCWSTR name);
      virtual void DeleteDataStandart(LPCWSTR name);

      virtual data_standart::iDataStandart* GetDataStandart(LPCWSTR name);

      virtual LPCWSTR GetBaseFolder() const;
      virtual void SetBaseFolder(LPCWSTR baseFolder);

      virtual LPCWSTR GetScriptFolder() const;
      virtual void SetScriptFolder(LPCWSTR scriptFolder);

      virtual void Clear();

   private:
      void clear();

      bool deserializeDataStandart(const _bstr_t& name/*, const fly_attrs_w& attrs*/);

      bool processDataStandart(LPCWSTR dataStandartName/*, const fly_attrs_w& dataSourceAttrs*/);
      data_standart::iDataStandart* createDataStandart(data_standart::DataStandartType type, LPCWSTR name);
   private:
      navigation_dispatcher::iComService* m_services;

      mutable std::unordered_map<std::wstring, StandartModuleGuard> m_dataStandartMap;
      //mutable std::map<_bstr_t, data_standart::iDataStandartPtr> m_dataStandartMap;

      std::wstring m_baseFolder;
      std::map<_bstr_t, _bstr_t> m_linksMap;
   };

private:
   mutable DataStandartFactoryImplInternal m_dataStandartImplInt;
};