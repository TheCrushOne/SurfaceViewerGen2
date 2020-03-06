#pragma once
#include "RenderLayers.h"


class TrackLayer : public LayersContainer
{
public:
   TrackLayer()
      : _step{ 1. }
   {}

   void SetStep(double step) { _step = step; }

   void Render(render::iRender* renderer) override
   {
      renderTracks(renderer);
   }

private:
   void renderTracks(render::iRender* renderer);

private:
   double _step;

   bool _renderSourceRoute = true;
   iPropertyPtr   _prop_renderSourceRoute;
};
