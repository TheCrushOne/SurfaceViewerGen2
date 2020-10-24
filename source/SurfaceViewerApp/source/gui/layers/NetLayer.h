#pragma once
#include "RenderLayers.h"

namespace SV
{
   class NetLayer : public LayersContainer
   {
   public:
      NetLayer()
         : _step{ 1. }
      {}

      void SetStep(double step) { _step = step; }

      void Render(render::iRender* renderer) override
      {
         createNet(renderer);
         createCells(renderer);
      }

   private:
      void createNet(render::iRender* renderer);
      void createCells(render::iRender* renderer);
   private:
      double _step;
   };
}