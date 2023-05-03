#pragma once

#include "Platform_State.h"
#include "Laser_Beam_Set.h"

class AsPlatform_Laser
{
public:
   ~AsPlatform_Laser();
   AsPlatform_Laser(AsPlatform_State &);

   void Init(AsLaser_Beam_Set *, AColor &, AColor &, AColor &);
   bool Act(EPlatform_State &, double);
   void Draw_State(HDC, double, RECT &);
   void Reset();
   void Fire(bool);

private:
   void Draw_Laser_Wing(HDC, double, bool);
   void Draw_Laser_Inner_Part(HDC, double);
   void Draw_Laser_Leg(HDC, double, bool);
   void Draw_Laser_Cabin(HDC, double);
   void Draw_Expanding_Figure(HDC, EFigure_Type, double, double, double, double, double, double, double, double, double);
   int Get_Expanding_Value(double, double, double);
   double Get_Gun_Pos(double, bool );

   bool Enable_Laser_Firing;
   int Laser_Transformation_Step;
   int Last_Laser_Shot_Tick;
   AsPlatform_State *Platform_State;
   AColor *Circle_Color, *Inner_Color; // UNO
   AColor *Gun_Color;

   AsLaser_Beam_Set *Laser_Beam_Set; // UNO 

   static const int Max_Laser_Transformation_Step = 20;
   static const int Laser_Shot_Timeout = AsConfig::FPS / 2; // 2 times per second
};
//-----------------------------------------------------------------------------------------------------------------------------------------------