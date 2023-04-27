#pragma once

#include "Falling_Letter.h"
#include "Ball_Set.h"

enum class EPlatform_State : unsigned char
{
   Unknown,

   Regular,
   Meltdown,
   Rolling,
   Glue,
   Expanding,
   Laser
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

enum class EPlatform_Transformation : unsigned char
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

enum class EFigure_Type : unsigned char
{
   Ellipse,
   Rectangle,
   Round_Rect_3x
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AsPlatform_State
{
public:
   AsPlatform_State();

   operator EPlatform_State() const;
   void operator = (EPlatform_State new_state);

   void Set_Next_State(EPlatform_State);
   EPlatform_State Get_Next_State();
   EPlatform_State Set_State(EPlatform_Substate_Regular);
   EPlatform_State Set_Next_Or_Regular_State(EPlatform_Substate_Regular);

   EPlatform_Substate_Regular Regular;
   EPlatform_Substate_Meltdown Meltdown;
   EPlatform_Substate_Rolling Rolling;
   EPlatform_Transformation Glue;
   EPlatform_Transformation Expanding;
   EPlatform_Transformation Laser;

   EPlatform_Moving_State Moving;

private:
   EPlatform_State Current_State;
   EPlatform_State Next_State; // We enter this state from AsPlatform::Set_State(EPlatform_Substate_Regular new_regular_state)
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AsPlatform_Glue
{
public:
   AsPlatform_Glue(AsPlatform_State &platform_state);

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

class AsPlatform_Expanding
{
public:
   ~AsPlatform_Expanding();
   AsPlatform_Expanding(AsPlatform_State &platform_state);

   void Init(AColor &, AColor &, AColor &);
   bool Act(double &, EPlatform_State &, bool &);
   void Draw_State(HDC, double);
   void Draw_Circle_Highlight(HDC, int, int);
   void Reset();

   double Expanding_Platform_Width;

private:
   void Draw_Expanding_Platform_Ball(HDC, double, bool);
   void Draw_Expanding_Truss(HDC, RECT &, bool);

   AsPlatform_State *Platform_State;
   AColor *Highlight_Color, *Circle_Color, *Inner_Color; // UNO
   AColor *Truss_Color;

   static const double Max_Expanding_Platform_Width, Min_Expanding_Platform_Width, Expanding_Platform_Width_Step;
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
   void Redraw_Platform();
   void Move(bool, bool);
   void On_Space_Key(bool);
   bool Hit_By(AFalling_Letter *);
   double Get_Middle_Pos();

   static const int Normal_Width = 28;
   static const int Circle_Size = 7;
   static const int Height = 7;
   static const int Normal_Platform_Inner_Width = Normal_Width - Circle_Size;
   static const int Expanding_Platform_Inner_Width = 12;

private:
   bool Set_Transformation_State(EPlatform_State, EPlatform_Transformation &);
   void Act_For_Meltdown_State();
   void Act_For_Rolling_State();
   void Act_For_Laser_State();
   void Draw_Normal_State(HDC, RECT &);
   void Draw_Meltdown_State(HDC, RECT &);
   void Draw_Rolling_State(HDC, RECT &);
   void Draw_Roll_In_State(HDC, RECT &);
   void Draw_Laser_State(HDC , RECT &);
   void Draw_Laser_Wing(HDC, bool);
   void Draw_Laser_Inner_Part(HDC);
   void Draw_Laser_Leg(HDC, bool);
   void Draw_Laser_Cabin(HDC);
   void Draw_Expanding_Figure(HDC, EFigure_Type, double, double, double, double, double, double, double, double, double);
   int Get_Expanding_Value(double, double, double);
   bool Reflect_On_Circle(double, double, double, ABall *);
   bool Get_Platform_Image_Stroke_Color(int, int, const AColor **, int &);
   void Get_Normal_Platform_Image(HDC);
   double Get_Current_Width();
   bool Correct_Platform_Pos();

   AsPlatform_State Platform_State;
   bool Left_Key_Down, Right_Key_Down;
   int Inner_Width;
   int Rolling_Step;
   int Laser_Transformation_Step;
   int Last_Redraw_timer_Tick;
   double X_Pos;
   double Speed;
   AsBall_Set *Ball_Set;
   AsPlatform_Glue Platform_Glue;
   AsPlatform_Expanding Platform_Expanding;

   int Normal_Platform_Image_Width, Normal_Platform_Image_Height;
   int *Normal_Platform_Image; // Platform image pixels on background

   int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

   RECT Platform_Rect, Prev_Platform_Rect;

   AColor Highlight_Color, Platform_Circle_Color, Platform_Inner_Color, Gun_Color;

   static const int Max_Laser_Transformation_Step = 20;
   static const int Meltdown_Speed = 3;
   static const int Max_Rolling_Step = 16;
   static const int Roll_In_Platform_End_X_Pos = 99;
   static const int Rolling_Platform_Speed = 3;
   static const int X_Step = 6;
};
