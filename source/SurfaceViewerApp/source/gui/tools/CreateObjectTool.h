#pragma once
#include "MouseTool.h"

namespace SV
{
   class CreateObjectTool : public BaseTool
   {
   public:
      CreateObjectTool(render::iRender* renderer/*, colreg::OBJECT_TYPE type*/)
         : BaseTool{ renderer }
         //, _type{ type }
      {
         /*if (_type == colreg::OBJECT_TYPE::OT_ALL)
            _createType = CREATE_TYPE::CT_SIMULATION_ROUTE;
         else if (_type == colreg::OBJECT_TYPE::OT_NO_GO_AREAS)
            _createType = CREATE_TYPE::CT_SOURCE_ROUTE;
         else if (_type == colreg::OBJECT_TYPE::OT_NONE)
            _createType = CREATE_TYPE::CT_SHIP;
         else
            _createType = CREATE_TYPE::CT_CHART_OBJ;*/
      }
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
         //renderObject();
         BaseTool::MouseWheele(zDelta, point);
      }
   private:
      /*void renderObject();
      void renderShip();
      void addObject();*/
   private:

      /*enum class CREATE_TYPE
      {
         CT_SHIP = 0,
         CT_SOURCE_ROUTE,
         CT_SIMULATION_ROUTE,
         CT_CHART_OBJ
      };
      CREATE_TYPE _createType;

      colreg::OBJECT_TYPE _type;
      geo_points _points;
      geo_point _pointLast;
      colreg::track_point_info _shipPos;
      std::vector<colreg::route_point> _route;*/
   };
}