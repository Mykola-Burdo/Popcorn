#pragma once

#include "Config.h"

enum class EBall_State
{
   EBS_Disabled, // Disabled (not drawn, not moved, not interacted)

   EBS_Normal,
   EBS_Lost,
   EBS_On_Platform,
   EBS_On_Parachute,
   EBS_Off_Parachute,
   EBS_Teleporting
};
//-----------------------------------------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
   virtual bool Check_Hit(double, double, ABall *) = 0;
   virtual bool Check_Hit(double, double);

   bool Hit_Circle_On_Line(double, double, double, double, double, double &);
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

class ABall : public AMover, public AGraphics_Object
{
public:
   ABall();

   virtual void Begin_Movement();
   virtual void Finish_Movement();
   virtual void Advance(double);
   virtual double Get_Speed();

   virtual void Act();
   virtual void Clear(HDC, RECT &);
   virtual void Draw(HDC, RECT &);
   virtual bool Is_Finished();

   void Set_Speed(double);
   void Draw_Teleporting(HDC, int);
   void Set_For_Test();
   bool Is_Test_Finished();
   EBall_State Get_State();
   void Set_State(EBall_State, double x_pos = 0, double y_pos = 0);
   void Get_Center(double &, double &);
   double Get_Direction();
   void Set_Direction(double);
   void Reflect(bool);
   bool Is_Moving_Up(); 
   bool Is_Moving_Left();
   void Set_On_Parachute(int, int);
   void Forced_Advance(double, double, double);
   void Release();

   int Release_Timer_Tick; // The value of the time counter after which the glued ball must be released

   static void Add_Hit_Checker(AHit_Checker *);

   static const double Radius;

private:
   void Redraw_Ball();
   void Redraw_Parachute();
   void Draw_Parachute(HDC, RECT &);
   void Clear_Parachute(HDC);

   EBall_State Ball_State, Prev_Ball_State;

   double Center_X_Pos, Center_Y_Pos;
   //double Rest_Distance;
   double Ball_Direction, Prev_Ball_Direction;
   double Ball_Speed, Prev_Ball_Speed;

   bool Testing_Is_Active;
   int Test_Iteration;
   double Rest_Test_Distance;

   RECT Ball_Rect, Prev_Ball_Rect;
   RECT Parachute_Rect, Prev_Parachute_Rect;

   static const int Parachute_Size = 15;
   static const int On_Platform_Timeout = 10 * AsConfig::FPS; // Time spent on the platform
   static int Hit_Checkers_Count;
   static AHit_Checker *Hit_Checkers[3];
};
