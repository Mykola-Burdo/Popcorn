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
const int Timer_ID = WM_USER + 1;

class AsEngine;
class ABall;
class ALevel;
class AsPlatform;
class AsBorder;

//-----------------------------------------------------------------------------------------------------------------------------------------------
class ABall
{
public:
   ABall();
   void Init();
   void Draw(HDC, RECT &, AsEngine *);
   void Move(AsEngine *, ALevel *, AsPlatform *);

   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   static const int Ball_Size = 4;

private:
   int Ball_X_Pos, Ball_Y_Pos;
   double Ball_Speed, Ball_Direction;

   RECT Ball_Rect, Prev_Ball_Rect;
};

class ALevel
{
public:
   void Init();
   void Check_Level_Brick_Hit(int &, double &);
   void Draw(HDC, RECT &);

   static const int Level_Width = 12; // Level width in cells
   static const int Level_Height = 14; // Level height in cells
   static const int Cell_Width = 16;
   static const int Cell_Height = 8;
   static const int Level_X_Offset = 8;
   static const int Level_Y_Offset = 6;

private:
   void Draw_Brick(HDC, int, int, EBrick_Type);
   void Set_Brick_Letter_Colors(bool, HPEN &, HBRUSH &, HPEN &, HBRUSH &);
   void Draw_Brick_Letter(HDC, int, int, int, EBrick_Type, ELetter_Type);

   HPEN Letter_Pen, Brick_Red_Pen, Brick_Blue_Pen;
   HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

   RECT Level_Rect;

   static const int Brick_Width = 15;
   static const int Brick_Height = 7;

};

//-----------------------------------------------------------------------------------------------------------------------------------------------
class AsPlatform 
{
public:
   AsPlatform();
   void Init();
   void Redraw_Platform(AsEngine *);
   void Draw(HDC, AsEngine *, RECT &);

   int X_Pos;
   int X_Step;
   int Width;

   static const int Y_Pos = 185;

private:
   int Inner_Width;

   RECT Platform_Rect, Prev_Platform_Rect;

   HPEN Highlight_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
   HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

   static const int Height = 7;
   static const int Circle_Size = 7;
};

//-----------------------------------------------------------------------------------------------------------------------------------------------
class AsBorder
{
public:
   void Init();
   void Draw(HDC, RECT &, AsEngine *);

   static const int Border_X_Offset = 6;
   static const int Border_Y_Offset = 4;

private:
   void Draw_Element(HDC, int, int y, bool, AsEngine *);

   HPEN Border_Blue_Pen, Border_White_Pen;
   HBRUSH Border_Blue_Brush, Border_White_Brush;
};

//-----------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
   AsEngine();
   void Init(HWND);
   void Draw_Frame(HDC, RECT &);
   int On_Key_Down(EKey_Type);
   int On_Timer();

   static const int Global_Scale = 3;

   static void Create_Pen_Brush(unsigned char, unsigned char, unsigned char, HPEN &, HBRUSH &);

   HWND Hwnd;
   HPEN BG_Pen;
   HBRUSH BG_Brush;

   static const int Max_X_Pos = ALevel::Level_X_Offset + ALevel::Cell_Width * ALevel::Level_Width;
   static const int Max_Y_Pos = 199 - ABall::Ball_Size;

private:
   ABall Ball;
   ALevel Level;
   AsPlatform Platform;
   AsBorder Border;
};