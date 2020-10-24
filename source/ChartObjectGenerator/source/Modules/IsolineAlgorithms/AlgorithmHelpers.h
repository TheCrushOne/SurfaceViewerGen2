#pragma once
#include <math.h>

namespace SV
{
   inline void HSVtoRGB(int H, double S, double V, int output[3])
   {
      double C = S * V;
      double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
      double m = V - C;
      double Rs, Gs, Bs;

      if (H >= 0 && H < 60)
      {
         Rs = C;
         Gs = X;
         Bs = 0;
      }
      else if (H >= 60 && H < 120)
      {
         Rs = X;
         Gs = C;
         Bs = 0;
      }
      else if (H >= 120 && H < 180)
      {
         Rs = 0;
         Gs = C;
         Bs = X;
      }
      else if (H >= 180 && H < 240)
      {
         Rs = 0;
         Gs = X;
         Bs = C;
      }
      else if (H >= 240 && H < 300)
      {
         Rs = X;
         Gs = 0;
         Bs = C;
      }
      else
      {
         Rs = C;
         Gs = 0;
         Bs = X;
      }

      output[0] = (Rs + m) * 255;
      output[1] = (Gs + m) * 255;
      output[2] = (Bs + m) * 255;
   }

   inline bool passedCheck(const pathfinder::SharedBoolMatrix& passedFlagMtx, int iRIdx, int iCIdx)
   {
      if (iRIdx < 0
         || iCIdx < 0
         || iRIdx >= passedFlagMtx->GetRowCount()
         || iCIdx >= passedFlagMtx->GetColCount())
         return false;
      return passedFlagMtx->Get(static_cast<size_t>(iRIdx), static_cast<size_t>(iCIdx));
   }

   inline bool inLineCheck(const pathfinder::SharedBoolMatrix& inLineFlagMtx, int iRIdx, int iCIdx)
   {
      if (iRIdx < 0
         || iCIdx < 0
         || iRIdx >= inLineFlagMtx->GetRowCount()
         || iCIdx >= inLineFlagMtx->GetColCount())
         return false;
      return inLineFlagMtx->Get(static_cast<size_t>(iRIdx), static_cast<size_t>(iCIdx));
   }

   inline bool activationCheck(const pathfinder::SharedBoolMatrix& actValMtx, int iRIdx, int iCIdx)
   {
      if (iRIdx < 0
         || iCIdx < 0
         || iRIdx >= actValMtx->GetRowCount()
         || iCIdx >= actValMtx->GetColCount())
         return false;
      return actValMtx->Get(static_cast<size_t>(iRIdx), static_cast<size_t>(iCIdx));
   }

   inline bool nearestActivationCheck(const pathfinder::SharedBoolMatrix& actValMtx, size_t rIdx, size_t cIdx)
   {
      size_t actCount = 0;
      int iRIdx = static_cast<int>(rIdx);
      int iCIdx = static_cast<int>(cIdx);
      actCount += activationCheck(actValMtx, iRIdx + 1, iCIdx - 1) ? 1 : 0;
      actCount += activationCheck(actValMtx, iRIdx + 1, iCIdx) ? 1 : 0;
      actCount += activationCheck(actValMtx, iRIdx + 1, iCIdx + 1) ? 1 : 0;
      actCount += activationCheck(actValMtx, iRIdx, iCIdx + 1) ? 1 : 0;
      actCount += activationCheck(actValMtx, iRIdx - 1, iCIdx + 1) ? 1 : 0;
      actCount += activationCheck(actValMtx, iRIdx - 1, iCIdx) ? 1 : 0;
      actCount += activationCheck(actValMtx, iRIdx - 1, iCIdx - 1) ? 1 : 0;
      actCount += activationCheck(actValMtx, iRIdx, iCIdx - 1) ? 1 : 0;
      return actCount < 7;
   }
}