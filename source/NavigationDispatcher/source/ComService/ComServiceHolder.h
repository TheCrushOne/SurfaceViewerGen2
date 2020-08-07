#pragma once

#include "common/holder_base.h"
#include "common\communicator.h"
#include "navdisp\ComService.h"
#include "ComServiceImpl.h"

namespace navigation_dispatcher
{
   // iCommandsProcessor guard
   class ComServiceHolder : public HolderBase<iComService>
   {
   public:
      ComServiceHolder(central_pack* pack)
         : HolderBase<iComService>(CreateCommandServices(pack))
      { }

      ~ComServiceHolder()
      {
         DestroyCommandServices(_pInterface);
      }
   };
}