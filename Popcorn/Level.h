#pragma once

#include "Config.h"
#include "Active_Brick.h"

enum class ELetter_Type
{
   ELT_None = 0,
   ELT_O
};

class ALevel
{
public:
   ALevel();
   void Init();
   void Check_Level_Brick_Hit(double &, double &);
   void Draw(HDC, RECT &);

   bool Has_Floor;
   AActive_Brick Active_Brick;

private:
   void Draw_Brick(HDC, int, int, EBrick_Type);
   void Set_Brick_Letter_Colors(bool, HPEN &, HBRUSH &, HPEN &, HBRUSH &);
   void Draw_Brick_Letter(HDC, int, int, int, EBrick_Type, ELetter_Type);

   HPEN Letter_Pen, Brick_Red_Pen, Brick_Blue_Pen;
   HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

   RECT Level_Rect;

   static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];
};