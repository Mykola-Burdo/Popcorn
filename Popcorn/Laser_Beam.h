#pragma once

#include "Config.h"

enum class ELaser_Beam_State : unsigned char
{
   Disabled,
   Active,
   Stopping,
   Cleanup
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class ALaser_Beam : public AMover, public AGraphics_Object
{
public:
   ALaser_Beam();

   virtual void Begin_Movement();
   virtual void Finish_Movement();
   virtual void Advance(double);
   virtual double Get_Speed();

   virtual void Act();
   virtual void Clear(HDC, RECT &);
   virtual void Draw(HDC, RECT &);
   virtual bool Is_Finished();

   void Set_At(double, double);
   bool Is_Active();

   static AHit_Checker_List Hit_Checker_List;

private:
   void Disable();
   void Redraw_Beam();

   ELaser_Beam_State Laser_Beam_State;
   double X_Pos, Y_Pos;
   double Speed;
   RECT Beam_Rect, Prev_Beam_Rect;

   static const int Width = 1;
   static const int Height = 3;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------