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
class ABall
{
public:
   ABall();
   void Init();
   void Draw(HDC, RECT &);
   void Move(ALevel *, int, int);
   EBall_State Get_State();
   void Set_State(EBall_State, int);

private:
   void Redraw_Ball();

   EBall_State Ball_State;

   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   double Ball_X_Pos, Ball_Y_Pos;
   double Ball_Direction;
   double Ball_Speed;

   RECT Ball_Rect, Prev_Ball_Rect;

   static const double Start_Ball_Y_Pos;
};
