#pragma once

#include "Laser_Beam.h"

class AsLaser_Beam_Set : public AMover, public AGraphics_Object
{
public:
   virtual void Begin_Movement();
   virtual void Finish_Movement();
   virtual void Advance(double);
   virtual double Get_Speed();

   virtual void Act();
   virtual void Clear(HDC, RECT &);
   virtual void Draw(HDC, RECT &);
   virtual bool Is_Finished();

   void Fire(double, double);

private:
   static const int Max_Laser_Beam_Count = 10;

   ALaser_Beam Laser_Beams[Max_Laser_Beam_Count];
};
//-----------------------------------------------------------------------------------------------------------------------------------------------