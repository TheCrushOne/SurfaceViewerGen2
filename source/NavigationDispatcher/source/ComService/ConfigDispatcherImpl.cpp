#include "stdafx.h"
#include "ConfigDispatcherImpl.h"
//#include "DataStandartManager\nd_command.hxx"
#include "common\utils.h"
//#include <memory>

using namespace navigation_dispatcher;

void ConfigDispatcherImpl::Dispatch(const wchar_t* configPath)
{
   xml_properties::PropertyContainer properties("NavigationDispatcherOrderConfig", "1.0");
   std::string path = SVGUtils::wstringToString(configPath);

   if (!properties.load(path.c_str()))
      return;

   const auto& root = properties.GetRoot();

   const auto commonParams = root.GetChild(tag::common);
   const auto orders = root.GetChild(tag::orders);
   const auto standarts = root.GetChild(tag::standarts);

   const auto& orderList = orders->GetChildren().equal_range(tag::order);
   for (auto iter = orderList.first; iter != orderList.second; ++iter)
      readOrder(iter->second);

   const auto& standartList = orders->GetChildren().equal_range(tag::standart);
   for (auto iter = standartList.first; iter != standartList.second; ++iter)
      readDataStandart(iter->second);
}

void ConfigDispatcherImpl::readDataStandart(const xml_properties::PropertyItem& standart)
{
   std::string type;
   std::wstring name;
   standart[tag::type].Get(type);
   standart[tag::name].Get(name);
   m_services->GetDataStandartFactory()->CreateDataStandart(data_standart::convert_datastandart_name(name.c_str()), name.c_str());
   m_services->GetDataStandartFactory()->GetDataStandart(name.c_str())->DeserializeAttrs(standart[tag::params]);
}

void ConfigDispatcherImpl::readOrder(const xml_properties::PropertyItem& order)
{
   std::string type;
   std::wstring name;
   order[tag::type].Get(type);
   order[tag::name].Get(name);
   m_services->GetOrderFactory()->CreateOrder(navigation_dispatcher::convert_command_name(name.c_str()));
   m_services->GetOrderFactory()->GetOrder()->
}