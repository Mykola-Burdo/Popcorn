#pragma once

#include <Windows.h>
#include "Border.h"
#include "Level.h"
#include "Platform.h"

enum class EKey_Type
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EGame_State
{
   EGS_Test_Ball,

   EGS_Play_Level,
   EGS_Lost_Ball,
   EGS_Restart_Level
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

const int Timer_ID = WM_USER + 1;
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AsBall_Set
{
public:
   void Draw(HDC, RECT &);
   void Release_From_Platform(double);
   void Set_On_Platform(double);
   bool All_Balls_Are_Lost();

   ABall Balls[AsConfig::Max_Balls_Count];
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AsEngine
{
public:
   AsEngine();
   void Init(HWND);
   void Draw_Frame(HDC, RECT &);
   int On_Key(EKey_Type, bool);
   int On_Timer();

private:
   void Play_Level();
   void Advance_Mover();
   void Act();
   void On_Falling_Letter(AFalling_Letter *);

   EGame_State Game_State;

   //ABall Ball;
   AsLevel Level;
   AsPlatform Platform;
   AsBorder Border;
   AsBall_Set Ball_Set;

   AMover *Movers[AsConfig::Max_Movers_Count];
};
