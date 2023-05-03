#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <cmath>

class ABall;
class AHit_Checker
{
public:
   virtual bool Check_Hit(double, double, ABall *) = 0;
   virtual bool Check_Hit(double, double);

   bool Hit_Circle_On_Line(double, double, double, double, double, double &);
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AHit_Checker_List
{
public:
   AHit_Checker_List();

   bool Add_Hit_Checker(AHit_Checker *);
   bool Check_Hit(double, double, ABall *);
   bool Check_Hit(double, double);

private:
   int Hit_Checkers_Count;
   AHit_Checker *Hit_Checkers[3];
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AMover
{
public:
   virtual ~AMover();

   virtual void Begin_Movement() = 0;
   virtual void Finish_Movement() = 0;
   virtual void Advance(double) = 0;
   virtual double Get_Speed() = 0;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AGraphics_Object
{
public:
   virtual ~AGraphics_Object();

   virtual void Act() = 0;
   virtual void Clear(HDC, RECT &) = 0;
   virtual void Draw(HDC, RECT &) = 0;
   virtual bool Is_Finished() = 0;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AColor
{
public:
   AColor();
   AColor(unsigned char, unsigned char, unsigned char);
   AColor(const AColor &, int);
   AColor(unsigned char, unsigned char, unsigned char, int);
   AColor(const AColor &, const AColor &, int);

   int Get_RGB() const;
   void Select(HDC) const;
   void Select_Pen(HDC) const;
   HBRUSH Get_Brush() const;

   unsigned char R, G, B;

private:
   HPEN Pen;
   HBRUSH Brush;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------