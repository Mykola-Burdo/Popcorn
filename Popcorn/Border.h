#pragma once

#include "Ball.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
class AsBorder : public AHit_Checker
{
public:
   AsBorder();

   virtual bool Check_Hit(double, double, ABall *);

   void Draw(HDC, RECT &);
   void Redraw_Floor();

private:
   void Draw_Element(HDC, RECT &, int, int y, bool);
   void Draw_Floor(HDC, RECT &);

   RECT Floor_Rect;
};