#pragma once

#include "Ball.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
class AGate : public AGraphics_Object
{
public:
   AGate(int, int);

   virtual void Act();
   virtual void Clear(HDC, RECT &);
   virtual void Draw(HDC, RECT &);
   virtual bool Is_Finished();

private:
   void Draw_Cup(HDC, bool);
   void Draw_Edges(HDC);
   void Draw_One_Edge(HDC, int, bool);

   int X_Pos, Y_Pos;
   int Edges_Count;
   RECT Gate_Rect;

   static const int Width = 6;
   static const int Height = 19;
};

//-----------------------------------------------------------------------------------------------------------------------------------------------
class AsBorder : public AHit_Checker, public AGraphics_Object
{
public:
   ~AsBorder();
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
   AGate *Gates[AsConfig::Gates_Count];
};