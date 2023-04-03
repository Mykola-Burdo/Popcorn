#pragma once

#include "Ball.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
class AsBorder : public AHit_Checker, public AGraphics_Object
{
public:
   AsBorder();

   virtual bool Check_Hit(double, double, ABall *);

   virtual void Act();
   virtual void Clear(HDC, RECT &);
   virtual void Draw(HDC, RECT &);
   virtual bool Is_Finished();

   void Redraw_Floor();

private:
   void Draw_Element(HDC, RECT &, int, int y, bool);
   void Draw_Floor(HDC, RECT &);

   RECT Floor_Rect;
};