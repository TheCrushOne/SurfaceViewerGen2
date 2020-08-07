#include "OrderProcessorImpl.h"
#include "common/utils.h"

void OrderProcessorImpl::ClearOrders()
{
   //m_loggers.clear();
   m_commands.clear();
}

void OrderProcessorImpl::AddOrder(navigation_dispatcher::iOrderPtr command)
{
   m_commands.insert(std::make_pair(std::to_wstring(m_commands.size()), command));
   //addLoggerForCommand(it.first->first.c_str(), command->GetType());
}

bool OrderProcessorImpl::ProcessOrders(LPCWSTR begCommandName)
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

bool OrderProcessorImpl::processOrder(LPCWSTR name, navigation_dispatcher::iOrderPtr& order)
{
   std::string orderType = SVGUtils::wstringToString(convert_order_type(order->GetType()));
   std::string orderTypeStrStart = orderType + "%S command started...";
   std::string orderTypeStrFailed = orderType + "%S command failed...";
   std::string orderTypeStrFinished = orderType + "%S command finished...";
   GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, orderTypeStrStart.c_str());
   bool result = order->Process();

   if (!result)
      GetPack()->comm->Message(ICommunicator::MessageType::MT_ERROR, orderTypeStrFailed.c_str());
   else
      GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, orderTypeStrFinished.c_str());

   //m_loggers[name].reset();
   // NOTE: shared_ptr dee
   order.reset();
   return result;
}
