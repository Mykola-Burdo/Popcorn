#pragma once

#include "Config.h"
#include "Level.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
class ABall
{
public:
   ABall();
   void Init();
   void Draw(HDC, RECT &);
   void Move(ALevel *, int, int);

private:
   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   int Ball_X_Pos, Ball_Y_Pos;
   double Ball_Speed, Ball_Direction;

   RECT Ball_Rect, Prev_Ball_Rect;
};
