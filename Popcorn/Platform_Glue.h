#pragma once

#include "Platform_State.h"
#include "Ball_Set.h"

class AsPlatform_Glue
{
public:
   AsPlatform_Glue(AsPlatform_State &);

   bool Act(AsBall_Set *, EPlatform_State &);
   void Draw_State(HDC, double);
   void Reset();

private:
   void Draw_Glue_Spot(HDC, double, int, int, int);

   double Glue_Spot_Height_Ratio;
   AsPlatform_State *Platform_State;

   static const double Max_Glue_Spot_Height_Ratio, Min_Glue_Spot_Height_Ratio, Glue_Spot_Height_Ratio_Step;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------