#pragma once

#include "Config.h"
#include "Level.h"

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
   void Move(int, int, ALevel*, AHit_Checker *);
   EBall_State Get_State();
   void Set_State(EBall_State, double);

   double Ball_Direction;

   static const double Radius;

private:
   void Redraw_Ball();

   EBall_State Ball_State;

   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   double Center_X_Pos, Center_Y_Pos;
   double Ball_Speed;
   double Rest_Distance;

   RECT Ball_Rect, Prev_Ball_Rect;

   static const double Start_Ball_Y_Pos;
};
