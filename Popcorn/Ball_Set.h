#pragma once

#include "Ball.h"

class AsBall_Set : public AMover, public AGraphics_Object
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

   void Release_From_Platform(double);
   void Release_Next_Ball();
   void Set_On_Platform(double);
   bool All_Balls_Are_Lost();
   void Set_For_Test();
   bool Is_Test_Finished();
   void Triple_Balls();
   void Inverse_Ball();
   void Accelerate();
   void Reset_Speed();

private:
   ABall Balls[AsConfig::Max_Balls_Count];
};
//-----------------------------------------------------------------------------------------------------------------------------------------------