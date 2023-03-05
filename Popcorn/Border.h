#pragma once

#include "Ball.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
class AsBorder : public AHit_Checker
{
public:
   AsBorder();

   virtual bool Check_Hit(double, double, ABall *);

   void Init();
   void Draw(HDC, RECT &);

private:
   void Draw_Element(HDC, int, int y, bool, HPEN, HBRUSH);

   HPEN Border_Blue_Pen, Border_White_Pen;
   HBRUSH Border_Blue_Brush, Border_White_Brush;
};