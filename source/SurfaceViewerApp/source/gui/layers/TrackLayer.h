#pragma once
#include "RenderLayers.h"


class TrackLayer : public LayersContainer
{
public:
   TrackLayer()
      : m_step{ 1. }
   {}

   void SetStep(double step) { m_step = step; }

   void Render(render::iRender* renderer) override
   {
      renderTracks(renderer);
   }

private:
   void renderTracks(render::iRender* renderer);

private:
   double m_step;

   bool m_renderSourceRoute = true;
   iPropertyPtr   m_prop_renderSourceRoute;
};
