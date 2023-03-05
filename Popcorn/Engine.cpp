#include "Engine.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
   : Game_State(EGame_State::EGS_Play_Level)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Init(HWND hwnd)
{// Game setup at startup

   AsConfig::Hwnd = hwnd;

   AActive_Brick::Setup_Colors();

   Level.Init();
   Platform.Init();
   Ball.Init();
   Border.Init();

   Ball.Set_State(EBall_State::EBS_Normal, Platform.X_Pos + Platform.Width / 2);

   Platform.Set_State(EPlatform_State::EPS_Normal);
   
   Platform.Redraw_Platform();

   SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Drawing the game screen

   Level.Draw(hdc, paint_area);

   /*
   for (int i = 0; i < 16; ++i)
   {
      Draw_Brick_Letter(hdc, 650, (AsConfig::Level_Y_Offset + 36) + i * AsConfig::Cell_Height * AsConfig::Global_Scale, i, EBrick_Type::EBT_Red, ELetter_Type::ELT_O);
   }
   */

   Ball.Draw(hdc, paint_area);

   Border.Draw(hdc, paint_area);

   Platform.Draw(hdc, paint_area);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKey_Type::EKT_Left:
      Platform.X_Pos -= Platform.X_Step;

      if (Platform.X_Pos <= AsConfig::Border_X_Offset)
         Platform.X_Pos = AsConfig::Border_X_Offset;

      Platform.Redraw_Platform();
      break;


   case EKey_Type::EKT_Right:
      Platform.X_Pos += Platform.X_Step;

      if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 1)
         Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;

      Platform.Redraw_Platform();
      break;


   case EKey_Type::EKT_Space:
      if(Platform.Get_State() == EPlatform_State::EPS_Ready)
      {
         Ball.Set_State(EBall_State::EBS_Normal, Platform.X_Pos + Platform.Width / 2);
         Platform.Set_State(EPlatform_State::EPS_Normal);
      }
      break;
   }

   return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

int AsEngine::On_Timer()
{
   ++AsConfig::Current_Timer_Tick;

   switch (Game_State)
   {
   case EGame_State::EGS_Play_Level:
      Ball.Move(&Level, Platform.X_Pos, Platform.Width);
      
      if(Ball.Get_State() == EBall_State::EBS_Lost)
      {
         Game_State = EGame_State::EGS_Lost_Ball;
         Platform.Set_State(EPlatform_State::EPS_Meltdown);
      }
      break;


   case EGame_State::EGS_Lost_Ball:
      if(Platform.Get_State() == EPlatform_State::EPS_Missing)
      {
         Game_State = EGame_State::EGS_Restart_Level;
         Platform.Set_State(EPlatform_State::EPS_Roll_In);
      }
      break;

   case EGame_State::EGS_Restart_Level:
      if (Platform.Get_State() == EPlatform_State::EPS_Ready)
      {
         Game_State = EGame_State::EGS_Play_Level;
         Ball.Set_State(EBall_State::EBS_On_Platform, Platform.X_Pos + Platform.Width / 2);
      }
      break;
   }

   Platform.Act();

   //Level.Active_Brick.Act();

   return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
