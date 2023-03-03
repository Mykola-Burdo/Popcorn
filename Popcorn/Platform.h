#pragma once

#include "Config.h"

enum class EPlatform_State
{
   EPS_Missing = 0,
   EPS_Normal,
   EPS_Meltdown
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AsPlatform
{
public:
   AsPlatform();
   void Init();
   void Act(HWND);
   void Redraw_Platform(HWND);
   void Draw(HDC, RECT &);

   int X_Pos;
   int X_Step;
   int Width;

private:
   void Draw_Normal_State(HDC, RECT &);
   void Draw_Meltdown_State(HDC, RECT &);

   EPlatform_State Platform_State;
   int Inner_Width;

   static const int Normal_Width = 28;

   int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

   RECT Platform_Rect, Prev_Platform_Rect;

   HPEN Highlight_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
   HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

   static const int Height = 7;
   static const int Circle_Size = 7;
   static const int Meltdown_Speed = 3;
};