#pragma once

#include "Falling_Letter.h"
#include "Ball.h"

class AsLevel : public AHit_Checker
{
public:
   AsLevel();

   virtual bool Check_Hit(double, double, ABall*);

   void Init();
   void Set_Current_Level(char level[AsConfig::Level_Height][AsConfig::Level_Width]);
   void Act();
   void Draw(HDC, RECT &);
   bool Get_Next_Falling_Letter(int &, AFalling_Letter **);

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