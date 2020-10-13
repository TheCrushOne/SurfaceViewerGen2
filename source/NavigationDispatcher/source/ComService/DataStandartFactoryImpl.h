#pragma once

#include "datastandart/DataStandartFactory.h"
#include "navdisp/ComService.h"
#include "colreg/ModuleGuard.h"
#include <comdef.h>
#include <map>

using StandartModuleGuard = colreg::ModuleGuard<data_standart::iDataStandart, central_pack*, LPCSTR, navigation_dispatcher::iComService*>;

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
   virtual data_standart::iDataStandart* CreateDataStandart(data_standart::DataStandartType type, LPCSTR name);
   virtual void DeleteDataStandart(LPCSTR name);

   virtual data_standart::iDataStandart* GetDataStandart(LPCSTR name) const;

   virtual LPCSTR GetBaseFolder() const;
   virtual void SetBaseFolder(LPCSTR baseFolder);

   virtual LPCSTR GetScriptFolder() const;
   virtual void SetScriptFolder(LPCSTR scriptFolder);

   virtual void Clear();

   class DataStandartFactoryImplInternal : public Central
   {
   public:
      DataStandartFactoryImplInternal(central_pack* pack, navigation_dispatcher::iComService* pService)
         : Central(pack)
         , m_services(pService)
      {
      }

      virtual data_standart::iDataStandart* CreateDataStandart(data_standart::DataStandartType type, LPCSTR name);
      virtual void DeleteDataStandart(LPCSTR name);

      virtual data_standart::iDataStandart* GetDataStandart(LPCSTR name);

      virtual LPCSTR GetBaseFolder() const;
      virtual void SetBaseFolder(LPCSTR baseFolder);

      virtual LPCSTR GetScriptFolder() const;
      virtual void SetScriptFolder(LPCSTR scriptFolder);

      virtual void Clear();

   private:
      void clear();

      bool deserializeDataStandart(const std::string& name/*, const fly_attrs_w& attrs*/);

      bool processDataStandart(LPCSTR dataStandartName/*, const fly_attrs_w& dataSourceAttrs*/);
      data_standart::iDataStandart* createDataStandart(data_standart::DataStandartType type, LPCSTR name);
   private:
      navigation_dispatcher::iComService* m_services;

      mutable std::unordered_map<std::string, StandartModuleGuard> m_dataStandartMap;
      //mutable std::map<_bstr_t, data_standart::iDataStandartPtr> m_dataStandartMap;

      std::string m_baseFolder;
      std::map<std::string, std::string> m_linksMap;
   };

private:
   mutable DataStandartFactoryImplInternal m_dataStandartImplInt;
};