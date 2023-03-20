#pragma once

#include "Config.h"

enum class EBall_State
{
   EBS_Normal,
   EBS_Lost,
   EBS_On_Platform,
   EBS_On_Parachute,
   EBS_Off_Parachute
};
//-----------------------------------------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
   virtual bool Check_Hit(double, double, ABall *) = 0;
   bool Hit_Circle_On_Line(double, double, double, double, double, double &);
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class ABall
{
public:
   ABall();
   void Draw(HDC, RECT &);
   void Move();
   void Set_For_Test();
   bool Is_Test_Finished();
   EBall_State Get_State();
   void Set_State(EBall_State, double, double y_pos = Start_Ball_Y_Pos);
   double Get_Direction();
   void Set_Direction(double);
   void Reflect(bool);
   bool Is_Moving_Up(); 
   bool Is_Moving_Left();
   void Set_On_Parachute(int, int);

   static void Add_Hit_Checker(AHit_Checker *);

   double Ball_Speed;

   static const double Radius;

private:
   void Redraw_Ball();
   void Redraw_Parachute();
   void Draw_Parachute(HDC, RECT &);
   void Clear_Parachute(HDC);

   EBall_State Ball_State, Prev_Ball_State;

   double Center_X_Pos, Center_Y_Pos;
   double Rest_Distance;
   double Ball_Direction;

   bool Testing_Is_Active;
   int Test_Iteration;
   double Rest_Test_Distance;

   RECT Ball_Rect, Prev_Ball_Rect;
   RECT Parachute_Rect, Prev_Parachute_Rect;

   static const double Start_Ball_Y_Pos;
   static const int Parachute_Size = 15;
   static int Hit_Checkers_Count;
   static AHit_Checker *Hit_Checkers[3];
};
