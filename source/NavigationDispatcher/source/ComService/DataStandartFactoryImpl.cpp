#include "stdafx.h"

#include "DataStandartFactoryImpl.h"

#include "datastandart/ChartObjectDataStandartInterface.h"
#include "datastandart/PackHoundDataStandartInterface.h"
#include "datastandart/PngHeightMapDataStandartInterface.h"
#include "datastandart/SVGenMapDataStandartInterface.h"
#include "DataStandart/PathStorageDataStandartInterface.h"
#include "datastandart/OptimizedPathStorageDataStandartInterface.h"

using namespace data_standart;

namespace DataStandartFactoryTags
{
   static LPCSTR TagParameters = "Parameters";
   static LPCSTR TagType = "Type";
   static LPCSTR TagDataStandarts = "Data sources";
   static LPCSTR TagLink = "Link";
   static LPCSTR TagBaseFolder = "Base folder";
}

iDataStandart* DataStandartFactoryImpl::CreateDataStandart(DataStandartType type, LPCSTR name)
{
   return m_dataStandartImplInt.CreateDataStandart(type, name);
}
void DataStandartFactoryImpl::DeleteDataStandart(LPCSTR name)
{
   m_dataStandartImplInt.DeleteDataStandart(name);
}

iDataStandart* DataStandartFactoryImpl::GetDataStandart(LPCSTR name) const
{
   return m_dataStandartImplInt.GetDataStandart(name);
}

LPCSTR DataStandartFactoryImpl::GetBaseFolder() const
{
   return m_dataStandartImplInt.GetBaseFolder();
}

void DataStandartFactoryImpl::SetBaseFolder(LPCSTR baseFolder)
{
   m_dataStandartImplInt.SetBaseFolder(baseFolder);
}

LPCSTR DataStandartFactoryImpl::GetScriptFolder() const
{
   return m_dataStandartImplInt.GetBaseFolder();
}

void DataStandartFactoryImpl::SetScriptFolder(LPCSTR scriptFolder)
{
   m_dataStandartImplInt.SetScriptFolder(scriptFolder);
}

void DataStandartFactoryImpl::Clear()
{
   m_dataStandartImplInt.Clear();
}

iDataStandart* DataStandartFactoryImpl::DataStandartFactoryImplInternal::CreateDataStandart(DataStandartType type, LPCSTR name)
{
   return createDataStandart(type, name);
}

void DataStandartFactoryImpl::DataStandartFactoryImplInternal::DeleteDataStandart(LPCSTR name)
{
   m_dataStandartMap.erase(name);
   m_linksMap.erase(name);
}

iDataStandart* DataStandartFactoryImpl::DataStandartFactoryImplInternal::GetDataStandart(LPCSTR name)
{
   auto itLink = m_linksMap.find(name);
   if (itLink != m_linksMap.end())
   {
      //fly_attrs_w itemAttrs;
      //m_services->GetDataSerializer()->DeserializeAttrs(itLink->second, itemAttrs);
      //if (!processDataStandart(name, itemAttrs))
         //MessageString(ICommunicator::MS_Error, "Can't read DataStandart %s from %s link file \n", (LPCSTR)_bstr_t(name), (LPCSTR)_bstr_t(itLink->second));
   }

   auto it = m_dataStandartMap.find(name);
   if (it == m_dataStandartMap.end())
      return nullptr;
   return it->second;
}

LPCSTR DataStandartFactoryImpl::DataStandartFactoryImplInternal::GetBaseFolder() const
{
   return m_baseFolder.c_str();
}

void DataStandartFactoryImpl::DataStandartFactoryImplInternal::SetBaseFolder(LPCSTR baseFolder)
{
   m_baseFolder = baseFolder;
}

LPCSTR DataStandartFactoryImpl::DataStandartFactoryImplInternal::GetScriptFolder() const
{
   return nullptr;
}

void DataStandartFactoryImpl::DataStandartFactoryImplInternal::SetScriptFolder(LPCSTR scriptFolder)
{
}

void DataStandartFactoryImpl::DataStandartFactoryImplInternal::Clear()
{
   clear();
}

void DataStandartFactoryImpl::DataStandartFactoryImplInternal::clear()
{
   m_dataStandartMap.clear();
   m_linksMap.clear();
}

bool DataStandartFactoryImpl::DataStandartFactoryImplInternal::deserializeDataStandart(const std::string &name/*, const fly_attrs_w& attrs*/)
{
   /*fly_attrs_w itemAttrs(attrs);
   if (!attrs.check_item(DataStandartFactoryTags::TagLink))
      return processDataStandart(name, itemAttrs);

   if (m_services->GetDataSerializer() == NULL)
   {
      assert(!"Can't resolve data source link without link resolver.");
      return false;
   }

   m_linksMap[name] = attrs.get_item(DataStandartFactoryTags::TagLink, L"");*/
   return true;
}

bool DataStandartFactoryImpl::DataStandartFactoryImplInternal::processDataStandart(LPCSTR DataStandartName/*, const fly_attrs_w& DataStandartAttrs*/)
{
   /*const _bstr_t typeStr = DataStandartAttrs.get_item(DataStandartFactoryTags::TagType, L"");

   const fly_attrs_w paramAttrs = DataStandartAttrs.get_branch(DataStandartFactoryTags::TagParameters);
   if (!paramAttrs.valid())
   {
      MessageString(ICommunicator::MS_Error, "Error! Bad data source '%s'. Can't find 'Parameters' section.\n", (LPCSTR)DataStandartName);
      return false;
   }

   DataStandartType type = convert_DataStandart_name(typeStr);
   iDataStandartPtr DataStandart = createDataStandart(type, DataStandartName);
   DataStandart->DeSerialize(paramAttrs);*/

   return true;
}

#define SWITCH_DS_CREATOR(type, creator) case type: m_dataStandartMap[name] = iDataStandartPtr(creator(GetPack(), m_baseFolder.c_str(), m_services)); break;

#define VALID_CHECK_DLL_LOAD(type, dllName, funcName/*, guard*/, ...) case type: \
   m_dataStandartMap[name] = StandartModuleGuard(); \
   m_dataStandartMap[name].Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName, __VA_ARGS__); \
   if (!m_dataStandartMap[name].IsValid()) \
   { \
      GetPack()->comm->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
   } \
   break;

iDataStandart* DataStandartFactoryImpl::DataStandartFactoryImplInternal::createDataStandart(DataStandartType type, LPCSTR name)
{
   //StandartModuleGuard obj;
   switch (type)
   {
      VALID_CHECK_DLL_LOAD(DST_PNGHM, "DataStandart", "CreatePngHeighMapDataStandart", GetPack(), "", m_services);
      VALID_CHECK_DLL_LOAD(DST_SVGM, "DataStandart", "CreateSurfaceViewerGenMapDataStandart", GetPack(), "", m_services);
      VALID_CHECK_DLL_LOAD(DST_OBJ, "DataStandart", "CreateChartObjectDataStandart", GetPack(), "", m_services);
      VALID_CHECK_DLL_LOAD(DST_PATHS, "DataStandart", "CreatePathStorageDataStandart", GetPack(), "", m_services);
      VALID_CHECK_DLL_LOAD(DST_OPTPATHS, "DataStandart", "CreateOptimizedPathStorageDataStandart", GetPack(), "", m_services);
      VALID_CHECK_DLL_LOAD(DST_PCKHND, "DataStandart", "CreatePackHoundDataStandart", GetPack(), "", m_services);
   default:
      GetPack()->comm->Message(ICommunicator::MessageType::MT_ERROR, "Error! Unknown type of data source '%s'.\n", (LPCSTR)_bstr_t(convert_datastandart_type(type)));
      return nullptr;
   }

   return m_dataStandartMap[name].operator->();
}

#undef SWITCH_DS_CREATOR
#undef VALID_CHECK_DLL_LOAD