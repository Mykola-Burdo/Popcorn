#pragma once

#include "Tools.h"

class AsConfig
{
public:
   static int Rand(int);
   static void Round_Rect(HDC, RECT &, int corner_radius = 2);
   static void Rect(HDC, RECT &, const AColor &);
   static void Rect(HDC, int, int, int, int, const AColor &);
   static void Invalidate_Rect(RECT &);
   static void Throw();

   static bool Level_Has_Floor;
   static int Current_Timer_Tick;
   static const AColor BG_Color, Red_Color, Blue_Color, White_Color, Letter_Color, Laser_Color, Gate_Color;
   static const AColor Unbreakable_Blue_Highlight, Unbreakable_Red_Highlight, Teleport_Portal_Color;
   static const AColor Advertisement_Blue_Table, Advertisement_Red_Table;
   static HWND Hwnd;

   static const double D_Global_Scale;
   static const double Moving_Step_Size;
   static const double Start_Ball_Y_Pos;
   static const double Ball_Acceleration;
   static const double Normal_Ball_Speed;
   static const double Min_Ball_Angle;

	static const int Global_Scale = 3;
   static const int Level_Width = 12; // Level width in cells
   static const int Level_Height = 14; // Level height in cells
   static const int Cell_Width = 16;
   static const int Cell_Height = 8;
   static const int Brick_Width = 15;
   static const int Brick_Height = 7;
   static const int Level_X_Offset = 8;
   static const int Level_Y_Offset = 6;
   static const int Border_X_Offset = 6;
   static const int Border_Y_Offset = 4;
   static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;
   static const int Max_Y_Pos = 199;
   static const int Platform_Y_Pos = 185;
   static const int FPS = 20;
   static const int Max_Active_Bricks_Count = 10;
   static const int Hits_Per_Letter = 1; // Probability of hitting a letter = 1.0 / Hits_Per_Letter
   static const int Max_Falling_Letters_Count = 10;
   static const int Max_Balls_Count = 10;
   static const int Max_Movers_Count = 10;
   static const int Max_Modules_Count = 10;
   static const int Initial_Life_Count = 5;
   static const int Max_Life_Count = 12;
   static const int Floor_Y_Pos = Max_Y_Pos - 1;
   static const int Platform_Normal_Width = 28;
   static const int Platform_Circle_Size = 7;
   static const int Platform_Height = 7;
   static const int Platform_Normal_Inner_Width = Platform_Normal_Width - Platform_Circle_Size;
   static const int Platform_Expanding_Inner_Width = 12;
   static const int Gates_Count = 8;
};
