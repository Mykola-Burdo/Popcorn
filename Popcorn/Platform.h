#pragma once

#include "Falling_Letter.h"
#include "Platform_State.h"
#include "Platform_Glue.h"
#include "Platform_Expanding.h"
#include "Platform_Laser.h"

class AsPlatform : public AHit_Checker, public AMover, public AGraphics_Object
{
public:
   ~AsPlatform();
   AsPlatform();

   virtual bool Check_Hit(double, double, ABall *);

   virtual void Begin_Movement();
   virtual void Finish_Movement();
   virtual void Advance(double);
   virtual double Get_Speed();

   virtual void Act();
   virtual void Clear(HDC, RECT &);
   virtual void Draw(HDC, RECT &);
   virtual bool Is_Finished();

   void Init(AsBall_Set*, AsLaser_Beam_Set *);
   EPlatform_State Get_State();
   void Set_State(EPlatform_State);
   void Set_State(EPlatform_Substate_Regular);
   bool Has_State(EPlatform_Substate_Regular);
   void Redraw_Platform();
   void Move(bool, bool);
   void On_Space_Key(bool);
   bool Hit_By(AFalling_Letter *);
   double Get_Middle_Pos();

private:
   bool Set_Transformation_State(EPlatform_State, EPlatform_Transformation &);
   void Act_For_Meltdown_State();
   void Act_For_Rolling_State();
   void Draw_Normal_State(HDC, RECT &);
   void Draw_Meltdown_State(HDC, RECT &);
   void Draw_Rolling_State(HDC, RECT &);
   void Draw_Roll_In_State(HDC, RECT &);
   bool Reflect_On_Circle(double, double, double, ABall *);
   bool Get_Platform_Image_Stroke_Color(int, int, const AColor **, int &);
   void Get_Normal_Platform_Image(HDC);
   double Get_Current_Width();
   bool Correct_Platform_Pos();

   AsPlatform_State Platform_State;
   bool Left_Key_Down, Right_Key_Down;
   int Inner_Width;
   int Rolling_Step;
   int Last_Redraw_timer_Tick;
   double X_Pos;
   double Speed;
   AsBall_Set *Ball_Set;
   AsPlatform_Glue Platform_Glue;
   AsPlatform_Expanding Platform_Expanding;
   AsPlatform_Laser Platform_Laser;

   int Normal_Platform_Image_Width, Normal_Platform_Image_Height;
   int *Normal_Platform_Image; // Platform image pixels on background

   int Meltdown_Platform_Y_Pos[AsConfig::Platform_Normal_Width * AsConfig::Global_Scale];

   RECT Platform_Rect, Prev_Platform_Rect;

   AColor Highlight_Color, Platform_Circle_Color, Platform_Inner_Color;

   static const int Meltdown_Speed = 3;
   static const int Max_Rolling_Step = 16;
   static const int Roll_In_Platform_End_X_Pos = 99;
   static const int Rolling_Platform_Speed = 3;
   static const int X_Step = 6;
};
