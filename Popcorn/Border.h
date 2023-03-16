#pragma once

#include "Ball.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
class AsBorder : public AHit_Checker
{
public:
   AsBorder();

   virtual bool Check_Hit(double, double, ABall *);

   void Draw(HDC, RECT &);

private:
   void Draw_Element(HDC, int, int y, bool);
};