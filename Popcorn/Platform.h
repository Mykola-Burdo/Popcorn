#pragma once

#include "Config.h"

enum class EPlatform_State
{
   EPS_Missing = 0,
   EPS_Normal,
   EPS_Meltdown,
   EPS_Roll_In,
   EPS_Expand_Roll_In
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AsPlatform
{
public:
   AsPlatform();
   void Init();
   void Act();
   void Set_State(EPlatform_State);
   void Redraw_Platform();
   void Draw(HDC, RECT &);

   int X_Pos;
   int X_Step;
   int Width;

private:
   void Clear_BG(HDC hdc);
   void Draw_Circle_Highlight(HDC, int, int);
   void Draw_Normal_State(HDC, RECT &);
   void Draw_Meltdown_State(HDC, RECT &);
   void Draw_Roll_In_State(HDC, RECT &);
   void Draw_Expanding_Roll_In_State(HDC, RECT &);

   EPlatform_State Platform_State;
   int Inner_Width;
   int Rolling_Step;

   static const int Normal_Width = 28;

   int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

   RECT Platform_Rect, Prev_Platform_Rect;

   HPEN Highlight_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
   HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

   static const int Height = 7;
   static const int Circle_Size = 7;
   static const int Normal_Platform_Inner_Width = Normal_Width - Circle_Size;
   static const int Meltdown_Speed = 3;
   static const int Max_Rolling_Step = 8;
   static const int Roll_In_Platform_End_X_Pos = 99;
   static const int Rolling_Platform_Speed = 3;
};