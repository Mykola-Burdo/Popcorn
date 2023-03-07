#pragma once

#include "Active_Brick.h"
#include "Ball.h"

enum class ELetter_Type
{
   ELT_None = 0,
   ELT_O
};

class ALevel : public AHit_Checker
{
public:
   ALevel();

   virtual bool Check_Hit(double, double, ABall*);

   void Init();
   void Set_Current_Level(char level[AsConfig::Level_Height][AsConfig::Level_Width]);
   void Draw(HDC, RECT &);

   //AActive_Brick Active_Brick;

   static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];
   static char Test_Level[AsConfig::Level_Height][AsConfig::Level_Width];

private:
   bool Check_Vertical_Hit(double, double, int, int, ABall *, double &);
   bool Check_Horizontal_Hit(double, double, int, int, ABall *, double &);
   bool Hit_Circle_On_Line(double, double, double, double, double, double &);
   void Draw_Brick(HDC, int, int, EBrick_Type);
   void Set_Brick_Letter_Colors(bool, HPEN &, HBRUSH &, HPEN &, HBRUSH &);
   void Draw_Brick_Letter(HDC, int, int, int, EBrick_Type, ELetter_Type);

   HPEN Letter_Pen, Brick_Red_Pen, Brick_Blue_Pen;
   HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

   RECT Level_Rect;

   double Current_Brick_Left_X, Current_Brick_Right_X;
   double Current_Brick_Top_Y, Current_Brick_Low_Y;

   char Current_Level[AsConfig::Level_Height][AsConfig::Level_Width];
};