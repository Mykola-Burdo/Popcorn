#pragma once

#include "Falling_Letter.h"

enum class EPlatform_State
{
   EPS_Missing,
   EPS_Ready,
   EPS_Normal,
   EPS_Meltdown,
   EPS_Roll_In,
   EPS_Expand_Roll_In
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EPlatform_Moving_State
{
   EPMS_Stop,
   EPMS_Moving_Left,
   EPMS_Moving_Right
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AsPlatform : public AHit_Checker
{
public:
   ~AsPlatform();
   AsPlatform();

   virtual bool Check_Hit(double, double, ABall *);

   void Act();
   EPlatform_State Get_State();
   void Set_State(EPlatform_State);
   void Redraw_Platform();
   void Draw(HDC, RECT &);
   void Move(bool, bool);
   bool Hit_By(AFalling_Letter *);
   void Advance(double);
   double Get_Middle_Pos();

   int Width;
   double Speed;

private:
   void Clear_BG(HDC hdc);
   void Draw_Circle_Highlight(HDC, int, int);
   void Draw_Normal_State(HDC, RECT &);
   void Draw_Meltdown_State(HDC, RECT &);
   void Draw_Roll_In_State(HDC, RECT &);
   void Draw_Expanding_Roll_In_State(HDC, RECT &);
   bool Reflect_On_Circle(double, double, double, ABall *);
   bool Get_Platform_Image_Stroke_Color(int, int, const AColor **, int &);
   void Get_Normal_Platform_Image(HDC);

   EPlatform_State Platform_State;
   EPlatform_Moving_State Platform_Moving_State;
   int Inner_Width;
   int Rolling_Step;
   double X_Pos;

   int Normal_Platform_Image_Width, Normal_Platform_Image_Height;
   int *Normal_Platform_Image; // Platform image pixels on background

   static const int Normal_Width = 28;

   int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

   RECT Platform_Rect, Prev_Platform_Rect;

   AColor Highlight_Color, Platform_Circle_Color, Platform_Inner_Color;

   static const int Height = 7;
   static const int Circle_Size = 7;
   static const int Normal_Platform_Inner_Width = Normal_Width - Circle_Size;
   static const int Meltdown_Speed = 3;
   static const int Max_Rolling_Step = 16;
   static const int Roll_In_Platform_End_X_Pos = 99;
   static const int Rolling_Platform_Speed = 3;
   static const int X_Step = AsConfig::Global_Scale * 2;
};
