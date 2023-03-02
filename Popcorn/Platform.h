#pragma once

#include "Config.h"

class AsPlatform
{
public:
   AsPlatform();
   void Init();
   void Redraw_Platform(HWND);
   void Draw(HDC, RECT &);

   int X_Pos;
   int X_Step;
   int Width;

private:
   int Inner_Width;

   RECT Platform_Rect, Prev_Platform_Rect;

   HPEN Highlight_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
   HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

   static const int Height = 7;
   static const int Circle_Size = 7;
};