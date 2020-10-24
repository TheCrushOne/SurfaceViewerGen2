#pragma once
#include "MouseTool.h"

namespace SV
{
   struct iSelected;

   class DragTool : public BaseTool
   {

   public:
      DragTool(render::iRender* renderer)
         :BaseTool{ renderer }
      {}
      void MouseMove(CPoint point) override;
      void LmouseDown(CPoint point) override;
      void LmouseUp(CPoint point) override;
      void MmouseDown(CPoint point) override;
      void MmouseUp(CPoint point) override;
      void RmouseDown(CPoint point) override;
      void RmouseUp(CPoint point) override;
      void MouseLeave() override;
   private:
      bool processFindInfo(const render::find_info& find);

   private:
      iSelected* m_selected = nullptr;
   };
}
