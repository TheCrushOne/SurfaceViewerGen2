#pragma once
#include "MouseTool.h"


class RulerTool : public BaseTool
{

public:
   RulerTool(render::iRender* renderer)
      : BaseTool{ renderer }
   {}
   void MouseMove(CPoint point)override;
   void LmouseDown(CPoint point) override;
   void LmouseUp(CPoint point) override;
   void MmouseDown(CPoint point) override;
   void MmouseUp(CPoint point)override;
   void RmouseDown(CPoint point) override;
   void RmouseUp(CPoint point) override;
   void MouseLeave()override;
   void MouseWheele(short zDelta, CPoint point)override
   {
      renderRuler(_clickPos, point);
      BaseTool::MouseWheele(zDelta, point);
   }
private:
   void renderRuler(CPoint from, CPoint to)const;
   std::string getDistText(double dist)const;
};


