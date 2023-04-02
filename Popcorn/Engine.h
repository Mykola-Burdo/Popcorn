#pragma once

#include <Windows.h>
#include "Border.h"
#include "Level.h"
#include "Platform.h"
#include "Ball_Set.h"

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
   void Advance_Movers();
   void Act();
   void On_Falling_Letter(AFalling_Letter *);

   EGame_State Game_State;
   double Rest_Distance;
   int Life_Count;

   //ABall Ball;
   AsLevel Level;
   AsPlatform Platform;
   AsBorder Border;
   AsBall_Set Ball_Set;

   AMover *Movers[AsConfig::Max_Movers_Count];
};
