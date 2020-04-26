#pragma once

#include <vector>
#include <windows.h>
#include "colreg\CommonStructs.h"

typedef DWORD COLORREF;

namespace user_interface
{
   enum OUTPUT_TYPE
   {
      OT_INFO = 0,
      OT_ERROR,
      OT_WARNING,
      OT_DEBUG,
      OT_OUTPUT,
      OT_PERFORMANCE,
      OT_SIZE
   };


   enum class EDIT_MODE
   {
      EM_NONE = 0,
      EM_DRAG,
      EM_MEASURMENT,
      EM_CREATE,
      EM_EDIT
   };

   void InvalidateView();
   void SetStatusBarText(const wchar_t* text);
   void SetProgress(unsigned int progress);
   void ShowToolTip(const wchar_t* title, const wchar_t* description);
   void HideToolTip();
   void SetCenter(double lt, double ln);
   void RaiseError();

   void SetOutputText(OUTPUT_TYPE type, const char* text, COLORREF color = 0, bool bold = false, bool italic = false);

   void SetEditMode(EDIT_MODE mode, unsigned long long int userData = 0);
   EDIT_MODE GetEditMode();

   union user_info
   {
      struct
      {
         char type;
         char data;
         short index;
      };
      size_t value;
   };

   struct objects_to_draw
   {
      std::vector<colreg::geo_point> units;
      std::vector<std::vector<colreg::geo_point>> chart_objects;
   };

   objects_to_draw GetObjectsInsideScreen();

   constexpr int selectedAlpha = 50;
   constexpr COLORREF selectedColor = RGB(50, 50, 255);
}