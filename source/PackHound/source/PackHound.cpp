#include "stdafx.h"
#include "PackHound.h"
#include "navdisp/OrderBase.h"
#include "navdisp\ComService.h"
#include "datastandart\PackHoundDataStandartInterface.h"

using namespace pack_hound;

PackHound::PackHound(central_pack* pack, navigation_dispatcher::iComService* pService)
   : navigation_dispatcher::OrderBase<navigation_dispatcher::OrderType::OT_PACKHOUND, navigation_dispatcher::packhound_order>(pack, pService)
{}

bool PackHound::processCommand()
{
   //auto dest = reinterpret_cast<data_standart::iPackHoundConfigDataStandart*>(dst);

   return true;
}

navigation_dispatcher::iOrder* CreatePackHound(central_pack* pack, navigation_dispatcher::iComService* pService)
{
   return new PackHound(pack, pService);
}