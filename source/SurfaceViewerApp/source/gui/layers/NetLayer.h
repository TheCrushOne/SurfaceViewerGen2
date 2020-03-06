#pragma once
#include "RenderLayers.h"


class NetLayer : public LayersContainer
{
public:
   NetLayer()
      : _step{ 1.}
   {}

   void SetStep(double step) { _step = step; }

   void Render(render::iRender*renderer ) override
   {
      createNet(renderer);
   }

private:
   void createNet(render::iRender*renderer);

private:
   double _step;
};
