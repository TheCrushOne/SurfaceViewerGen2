#pragma once

#include "common/holder_base.h"
#include "common\communicator.h"
#include "navdisp\ComService.h"
#include "ComServiceImpl.h"

namespace SV::navigation_dispatcher
{
   // iCommandsProcessor guard
   class ComServiceHolder : public HolderBase<iComService>
   {
   public:
      ComServiceHolder(central_pack* pack, const char* baseFolder)
         : HolderBase<iComService>(CreateCommandServices(pack, baseFolder))
      { }

      ~ComServiceHolder()
      {
         DestroyCommandServices(_pInterface);
      }
   };
}