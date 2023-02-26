#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES
#include<cmath>

enum class ELetter_Type
{
   ELT_None = 0,
   ELT_O
};

enum class EBrick_Type
{
   EBT_None = 0,
   EBT_Red,
   EBT_Blue
};

enum class EKey_Type
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};

const int Global_Scale = 3;
const int Timer_ID = WM_USER + 1;

class AsEngine
{
public:
   AsEngine();
   void Init_Engine(HWND);
   void Draw_Frame(HDC, RECT &);
   int On_Key_Down(EKey_Type);
   int On_Timer();

   static const int Level_Width = 12; // Level width in cells
   static const int Level_Height = 14; // Level height in cells

private:
   void Create_Pen_Brush(unsigned char, unsigned char, unsigned char, HPEN &, HBRUSH &);
   void Redraw_Platform();
   void Draw_Brick(HDC, int, int, EBrick_Type);
   void Set_Brick_Letter_Colors(bool, HPEN &, HBRUSH &, HPEN &, HBRUSH &);
   void Draw_Brick_Letter(HDC, int, int, int, EBrick_Type, ELetter_Type);
   void Draw_Level(HDC);
   void Draw_Platform(HDC, int, int);
   void Draw_Ball(HDC, RECT &);
   void Draw_Border(HDC, int, int y, bool);
   void Draw_Bounds(HDC, RECT &);
   void Check_Level_Brick_Hit(int &);
   void Move_Ball();

   HWND Hwnd;
   HPEN Highlight_Pen, Letter_Pen, BG_Pen, Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen,
      Platform_Inner_Pen, Ball_Pen, Border_Blue_Pen, Border_White_Pen;
   HBRUSH BG_Brush, Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush, Ball_Brush,
      Border_Blue_Brush, Border_White_Brush;

   int Inner_Width;
   int Platform_X_Pos;
   int Platform_X_Step;
   int Platform_Width;

   int Ball_X_Pos, Ball_Y_Pos;
   double Ball_Speed, Ball_Direction;

   RECT Platform_Rect, Prev_Platform_Rect;
   RECT Level_Rect;
   RECT Ball_Rect, Prev_Ball_Rect;

   static const int Brick_Width = 15;
   static const int Brick_Height = 7;
   static const int Cell_Width = 16;
   static const int Cell_Height = 8;
   static const int Level_X_Offset = 8;
   static const int Level_Y_Offset = 6;
   static const int Circle_Size = 7;
   static const int Platform_Y_Pos = 185;
   static const int Platform_Height = 7;
   static const int Ball_Size = 4;
   static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;
   static const int Max_Y_Pos = 199 - Ball_Size;
   static const int Border_X_Offset = 6;
   static const int Border_Y_Offset = 4;
};