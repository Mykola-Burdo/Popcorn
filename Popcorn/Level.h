#pragma once

#include "Active_Brick.h"
#include "Ball.h"

enum class ELetter_Type
{
   ELT_None = 0,
   ELT_O
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AFalling_Letter : public AGraphics_Object
{
public:
   AFalling_Letter(EBrick_Type, ELetter_Type, int, int);

   virtual void Act();
   virtual void Draw(HDC, RECT&);
   virtual bool Is_Finished();

   const EBrick_Type Brick_Type;
   const ELetter_Type Letter_Type;

   bool Got_Hit;

private:
   void Set_Brick_Letter_Colors(bool, HPEN &, HBRUSH &, HPEN &, HBRUSH &);
   void Draw_Brick_Letter(HDC);

   int X, Y;
   int Rotation_Step;
   int Next_Rotation_Tick;
   RECT Letter_Cell, Prev_Letter_Cell;

   static const int Ticks_Per_Step = 4;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class ALevel : public AHit_Checker
{
public:
   ALevel();

   virtual bool Check_Hit(double, double, ABall*);

   void Init();
   void Set_Current_Level(char level[AsConfig::Level_Height][AsConfig::Level_Width]);
   void Act();
   void Draw(HDC, RECT &);

   //AActive_Brick Active_Brick;

   static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];
   static char Test_Level[AsConfig::Level_Height][AsConfig::Level_Width];

private:
   void On_Hit(int, int);
   bool Add_Falling_Letter(int, int, EBrick_Type);
   void Add_Active_Brick(int, int, EBrick_Type);
   bool Check_Vertical_Hit(double, double, int, int, ABall *, double &);
   bool Check_Horizontal_Hit(double, double, int, int, ABall *, double &);
   void Draw_Brick(HDC, RECT &, EBrick_Type);
   void Draw_Objects(HDC, RECT&, AGraphics_Object **, int);
   void Act_Objects(AGraphics_Object **, int);

   RECT Level_Rect;

   double Current_Brick_Left_X, Current_Brick_Right_X;
   double Current_Brick_Top_Y, Current_Brick_Low_Y;

   char Current_Level[AsConfig::Level_Height][AsConfig::Level_Width];
   int Active_Bricks_Count;
   AActive_Brick *Active_Bricks[AsConfig::Max_Active_Bricks_Count]; 
   int Falling_Letters_Count;
   AFalling_Letter * Falling_Letters[AsConfig::Max_Falling_Letters_Count];
};