#pragma once

#include "Config.h"

enum class EBall_State
{
   EBS_Normal,
   EBS_Lost,
   EBS_On_Platform
};
//-----------------------------------------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
   virtual bool Check_Hit(double, double, ABall *) = 0;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class ABall
{
public:
   ABall();
   void Init();
   void Draw(HDC, RECT &);
   void Move();
   void Set_For_Test();
   bool Is_Test_Finished();
   EBall_State Get_State();
   void Set_State(EBall_State, double, double y_pos = Start_Ball_Y_Pos);
   double Get_Direction();
   void Set_Direction(double);
   void Reflect(bool);

   static void Add_Hit_Checker(AHit_Checker *);

   static const double Radius;

private:
   void Redraw_Ball();

   EBall_State Ball_State;

   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   double Center_X_Pos, Center_Y_Pos;
   double Ball_Speed;
   double Rest_Distance;
   double Ball_Direction;

   bool Testing_Is_Active;
   int Test_Iteration;
   double Rest_Test_Distance;

   RECT Ball_Rect, Prev_Ball_Rect;

   static const double Start_Ball_Y_Pos;
   static int Hit_Checkers_Count;
   static AHit_Checker *Hit_Checkers[3];
};
