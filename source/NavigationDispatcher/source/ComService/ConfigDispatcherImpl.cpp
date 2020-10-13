#include "stdafx.h"
#include "ConfigDispatcherImpl.h"
//#include "DataStandartManager\nd_command.hxx"
#include "common\utils.h"
//#include <memory>

using namespace navigation_dispatcher;

void ConfigDispatcherImpl::Dispatch(const char* configPath)
{
   xml_properties::PropertyContainer properties("NavigationDispatcherOrderConfig", "1.0");

   if (!properties.load(configPath))
      return;

   const auto& sClass = properties.GetRoot();
   const auto root = sClass.GetChild(tag::root);

   const auto commonParams = root->GetChild(tag::common);
   const auto orders = root->GetChild(tag::orders);
   const auto standarts = root->GetChild(tag::standarts);

   const auto& orderList = orders->GetChildren().equal_range(tag::order);
   for (auto iter = orderList.first; iter != orderList.second; ++iter)
      readOrder(iter->second);

   const auto& standartList = standarts->GetChildren().equal_range(tag::standart);
   for (auto iter = standartList.first; iter != standartList.second; ++iter)
      readDataStandart(iter->second);
}

void ConfigDispatcherImpl::readDataStandart(const xml_properties::PropertyItem& standart)
{
   std::string type, name;
   std::wstring wtype;
   standart[tag::type].Get(type);
   standart[tag::name].Get(name);
   wtype = SVGUtils::stringToWstring(type);
   auto* ds = m_services->GetDataStandartFactory()->CreateDataStandart(data_standart::convert_datastandart_name(wtype.c_str()), name.c_str());
   ds->DeserializeAttrs(standart[tag::params]);
}

void ConfigDispatcherImpl::readOrder(const xml_properties::PropertyItem& order)
{
   std::string type, name;
   std::wstring wtype;
   std::string id = order.GetAttribute(tag::id);
   order[tag::type].Get(type);
   order[tag::name].Get(name);
   wtype = SVGUtils::stringToWstring(type);
   auto* ord = m_services->GetOrderFactory()->CreateOrder(navigation_dispatcher::convert_command_name(wtype.c_str()), id.c_str());
   ord->DeserializeAttrs(order[tag::params]);
   m_services->GetOrderProcessor()->AddOrder(ord);
}