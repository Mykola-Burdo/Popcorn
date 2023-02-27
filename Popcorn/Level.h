#pragma once

#include "Config.h"

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

class ALevel
{
public:
   ALevel();
   void Init();
   void Check_Level_Brick_Hit(int &, double &);
   void Draw(HDC, RECT &);

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