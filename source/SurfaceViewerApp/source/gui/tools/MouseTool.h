#pragma once
#include "render\Render.h"
#include "gui/user_interface.h"

struct iMouseTool abstract
{
   virtual void MouseMove(CPoint point) = 0;
   virtual void LmouseDown(CPoint point) = 0;
   virtual void LmouseUp(CPoint point) = 0;
   virtual void MmouseDown(CPoint point) = 0;
   virtual void MmouseUp(CPoint point) = 0;
   virtual void RmouseDown(CPoint point) = 0;
   virtual void RmouseUp(CPoint point) = 0;
   virtual void MouseLeave() = 0;
   virtual void MouseWheele(short zDelta, CPoint point) = 0;
   virtual ~iMouseTool(void) = default;
};


using iMouseToolPtr = std::unique_ptr< iMouseTool>;

iMouseToolPtr create_mouse_tool(user_interface::EDIT_MODE type, render::iRender* renderer, unsigned long long int userData = 0);



class BaseTool : public iMouseTool
{

public:
   BaseTool(render::iRender* renderer)
      :_renderer{ renderer }
   {}
   void LmouseDown(CPoint point) override { _lbDown = true; _clickPos = point; }
   void LmouseUp(CPoint point) override { _lbDown = false; }
   void MmouseDown(CPoint point) override {}
   void MmouseUp(CPoint point) override {}
   void RmouseDown(CPoint point) override {}
   void RmouseUp(CPoint point) override {}
   void MouseMove(CPoint point) override;
   void MouseWheele(short zDelta, CPoint point)override;
   void MouseLeave()override {}
protected:
   render::iRender* _renderer;
   CPoint _clickPos;
   bool _lbDown = false;
   bool _moved = false;
};
