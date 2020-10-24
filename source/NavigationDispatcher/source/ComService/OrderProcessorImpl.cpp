#include "stdafx.h"
#include "OrderProcessorImpl.h"
#include "common/utils.h"

using namespace SV;
using namespace SV::navigation_dispatcher;

void OrderProcessorImpl::ClearOrders()
{
   m_commands.clear();
}

void OrderProcessorImpl::AddOrder(navigation_dispatcher::iOrder* command)
{
   m_commands.insert(std::make_pair(std::to_string(m_commands.size()), command));
}

bool OrderProcessorImpl::ProcessOrders(LPCSTR begCommandName)
{
   if (begCommandName != nullptr && m_commands.find(begCommandName) == m_commands.end())
   {
      GetPack()->comm->Message(ICommunicator::MessageType::MT_ERROR, "Can't find start command %S", begCommandName);
      return false;
   }

   auto begIt = begCommandName == nullptr ? m_commands.begin() : m_commands.find(begCommandName);
   for (auto it = begIt; it != m_commands.end(); ++it)
   {
      if (!processOrder(it->first.c_str(), it->second))
         return false;
   }

   return true;
}

bool OrderProcessorImpl::processOrder(LPCSTR name, navigation_dispatcher::iOrder* order)
{
   std::string orderType = SVGUtils::wstringToString(convert_order_type(order->GetType()));
   std::string orderTypeStrStart = orderType + "command started...";
   std::string orderTypeStrFailed = orderType + "command failed...";
   std::string orderTypeStrFinished = orderType + "command finished...";
   GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, orderTypeStrStart.c_str());
   bool result = order->Process();

   if (!result)
      GetPack()->comm->Message(ICommunicator::MessageType::MT_ERROR, orderTypeStrFailed.c_str());
   else
      GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, orderTypeStrFinished.c_str());

   return result;
}
