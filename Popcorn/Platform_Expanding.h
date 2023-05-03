#pragma once

#include "Platform_State.h"

class AsPlatform_Expanding
{
public:
   ~AsPlatform_Expanding();
   AsPlatform_Expanding(AsPlatform_State &);

   void Init(AColor &, AColor &, AColor &);
   bool Act(double &, EPlatform_State &, bool &);
   void Draw_State(HDC, double);
   void Draw_Circle_Highlight(HDC, int, int);
   void Reset();

   double Expanding_Platform_Width;

private:
   void Draw_Expanding_Platform_Ball(HDC, double, bool);
   void Draw_Expanding_Truss(HDC, RECT &, bool);

   AsPlatform_State *Platform_State;
   AColor *Highlight_Color, *Circle_Color, *Inner_Color; // UNO
   AColor *Truss_Color;

   static const double Max_Expanding_Platform_Width, Min_Expanding_Platform_Width, Expanding_Platform_Width_Step;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------