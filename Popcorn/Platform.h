#pragma once

#include "Falling_Letter.h"
#include "Ball_Set.h"

enum class EPlatform_State : unsigned char
{
   Regular,
   Meltdown,
   Rolling,
   Glue,
   Expanding
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EPlatform_Substate_Regular : unsigned char
{
   Unknown,

   Missing,
   Ready,
   Normal
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EPlatform_Substate_Meltdown : unsigned char
{
   Unknown,

   Init,
   Active
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EPlatform_Substate_Rolling : unsigned char
{
   Unknown,

   Roll_In,
   Expand_Roll_In,
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EPlatform_Substate_Glue : unsigned char
{
   Unknown,

   Init,
   Active,
   Finalize
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EPlatform_Substate_Expanding : unsigned char
{
   Unknown,

   Init,
   Active,
   Finalize
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EPlatform_Moving_State : unsigned char
{
   Stopping,
   Stop,
   Moving_Left,
   Moving_Right
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AsPlatform_State
{
public:
   AsPlatform_State();

   operator EPlatform_State() const;
   void operator = (EPlatform_State new_state);

   EPlatform_Substate_Regular Regular;
   EPlatform_Substate_Meltdown Meltdown;
   EPlatform_Substate_Rolling Rolling;
   EPlatform_Substate_Glue Glue;
   EPlatform_Substate_Expanding Expanding;

   EPlatform_Moving_State Moving;

private:
      EPlatform_State Current_State;
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
   void Set_State(EPlatform_Substate_Regular);
   bool Has_State(EPlatform_Substate_Regular);
   void Redraw_Platform(bool update_rect = true);
   void Move(bool, bool);
   void On_Space_Key(bool);
   bool Hit_By(AFalling_Letter *);
   double Get_Middle_Pos();

   int Width;

private:
   void Act_For_Meltdown_State();
   void Act_For_Rolling_State();
   void Act_For_Glue_State();
   void Act_For_Expanding_State();
   void Draw_Circle_Highlight(HDC, int, int);
   void Draw_Normal_State(HDC, RECT &);
   void Draw_Meltdown_State(HDC, RECT &);
   void Draw_Rolling_State(HDC, RECT &);
   void Draw_Roll_In_State(HDC, RECT &);
   void Draw_Glue_State(HDC, RECT &);
   void Draw_Glue_Spot(HDC, int, int, int);
   void Draw_Expanding_State(HDC, RECT &);
   void Draw_Expanding_Platform_Ball(HDC, bool);
   void Draw_Expanding_Truss(HDC, RECT &, bool);
   bool Reflect_On_Circle(double, double, double, ABall *);
   bool Get_Platform_Image_Stroke_Color(int, int, const AColor **, int &);
   void Get_Normal_Platform_Image(HDC);

   AsPlatform_State Platform_State;
   bool Left_Key_Down, Right_Key_Down;
   int Inner_Width;
   int Rolling_Step;
   double X_Pos;
   double Speed;
   double Glue_Spot_Height_Ratio;
   double Expanding_Platform_Width;
   AsBall_Set *Ball_Set;

   int Normal_Platform_Image_Width, Normal_Platform_Image_Height;
   int *Normal_Platform_Image; // Platform image pixels on background

   static const int Normal_Width = 28;

   int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

   RECT Platform_Rect, Prev_Platform_Rect;

   AColor Highlight_Color, Platform_Circle_Color, Platform_Inner_Color, Truss_Color;

   static const double Max_Glue_Spot_Height_Ratio, Min_Glue_Spot_Height_Ratio, Glue_Spot_Height_Ratio_Step;
   static const double Max_Expanding_Platform_Width, Min_Expanding_Platform_Width, Expanding_Platform_Width_Step;
   static const int Height = 7;
   static const int Circle_Size = 7;
   static const int Normal_Platform_Inner_Width = Normal_Width - Circle_Size;
   static const int Expanding_Platform_Inner_Width = 12;
   static const int Meltdown_Speed = 3;
   static const int Max_Rolling_Step = 16;
   static const int Roll_In_Platform_End_X_Pos = 99;
   static const int Rolling_Platform_Speed = 3;
   static const int X_Step = 6;
};
