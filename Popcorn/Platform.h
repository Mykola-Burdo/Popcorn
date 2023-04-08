#pragma once

#include "Falling_Letter.h"
#include "Ball_Set.h"

enum class EPlatform_State
{
   EPS_Missing,
   EPS_Ready,
   EPS_Normal,
   EPS_Pre_Meltdown,
   EPS_Meltdown,
   EPS_Roll_In,
   EPS_Expand_Roll_In,
   EPS_Glue,
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EPlatform_Substate_Glue
{
   EPSG_Unknown,

   EPSG_Init,
   EPSG_Active,
   EPSG_Finalize
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EPlatform_Moving_State
{
   EPMS_Stopping,
   EPMS_Stop,
   EPMS_Moving_Left,
   EPMS_Moving_Right
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

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

   void Init(AsBall_Set*);
   EPlatform_State Get_State();
   void Set_State(EPlatform_State);
   void Redraw_Platform(bool update_rect = true);
   void Move(bool, bool);
   void On_Space_Key(bool);
   bool Hit_By(AFalling_Letter *);
   double Get_Middle_Pos();

   int Width;

private:
   void Draw_Circle_Highlight(HDC, int, int);
   void Draw_Normal_State(HDC, RECT &);
   void Draw_Meltdown_State(HDC, RECT &);
   void Draw_Roll_In_State(HDC, RECT &);
   void Draw_Expanding_Roll_In_State(HDC, RECT &);
   void Draw_Glue_State(HDC, RECT &);
   void Draw_Glue_Spot(HDC, int, int, int);
   bool Reflect_On_Circle(double, double, double, ABall *);
   bool Get_Platform_Image_Stroke_Color(int, int, const AColor **, int &);
   void Get_Normal_Platform_Image(HDC);

   EPlatform_State Platform_State;
   EPlatform_Substate_Glue Platform_Substate_Glue;
   EPlatform_Moving_State Platform_Moving_State;
   bool Left_Key_Down, Right_Key_Down;
   int Inner_Width;
   int Rolling_Step;
   double X_Pos;
   double Speed;
   double Glue_Spot_Height_Ratio;
   AsBall_Set *Ball_Set;

   int Normal_Platform_Image_Width, Normal_Platform_Image_Height;
   int *Normal_Platform_Image; // Platform image pixels on background

   static const int Normal_Width = 28;

   int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

   RECT Platform_Rect, Prev_Platform_Rect;

   AColor Highlight_Color, Platform_Circle_Color, Platform_Inner_Color;

   static const double Max_Glue_Spot_Height_Ratio, Min_Glue_Spot_Height_Ratio, Glue_Spot_Height_Ratio_Step;
   static const int Height = 7;
   static const int Circle_Size = 7;
   static const int Normal_Platform_Inner_Width = Normal_Width - Circle_Size;
   static const int Meltdown_Speed = 3;
   static const int Max_Rolling_Step = 16;
   static const int Roll_In_Platform_End_X_Pos = 99;
   static const int Rolling_Platform_Speed = 3;
   static const int X_Step = 6;
};
