#include "Engine.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
   : Game_State(EGame_State::EGS_Lost_Ball)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Init(HWND hwnd)
{// Game setup at startup

   SYSTEMTIME sys_time;
   FILETIME file_time;

   GetSystemTime(&sys_time);
   SystemTimeToFileTime(&sys_time, &file_time);

   srand(file_time.dwLowDateTime);

   AsConfig::Hwnd = hwnd;

   AActive_Brick_Red_Blue::Setup_Colors();

   Level.Init();

   AFalling_Letter::Init();

   ABall::Add_Hit_Checker(&Border);
   ABall::Add_Hit_Checker(&Level);
   ABall::Add_Hit_Checker(&Platform);

   Level.Set_Current_Level(AsLevel::Level_01);

   //Ball.Set_State(EBall_State::EBS_Normal, Platform.X_Pos + Platform.Width / 2);

   //Platform.Set_State(EPlatform_State::EPS_Normal);
   
   Platform.Redraw_Platform();

   SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Drawing the game screen

   SetGraphicsMode(hdc, GM_ADVANCED);

   Level.Draw(hdc, paint_area);

   Border.Draw(hdc, paint_area);

   Platform.Draw(hdc, paint_area);

   for(int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Draw(hdc, paint_area);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKey_Type::EKT_Left:
      Platform.Move(true);
      break;


   case EKey_Type::EKT_Right:
      /*Platform.X_Pos += Platform.X_Step;

      if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 1)
         Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;

      Platform.Redraw_Platform();*/

      Platform.Move(false);

      break;


   case EKey_Type::EKT_Space:
      if(Platform.Get_State() == EPlatform_State::EPS_Ready)
      {
         for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
            if(Balls[i].Get_State() == EBall_State::EBS_On_Platform)
               Balls[i].Set_State(EBall_State::EBS_Normal, Platform.X_Pos + Platform.Width / 2, AsConfig::Start_Ball_Y_Pos);

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
   case EGame_State::EGS_Test_Ball:
      Balls[0].Set_For_Test(); // Only the ball with index 0 participates in repeated tests
      Game_State = EGame_State::EGS_Play_Level;
      break;


   case EGame_State::EGS_Play_Level:
      Play_Level();
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
         Restart_Level();
      break;
   }

   Act();

   return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Restart_Level()
{
   int i;

   Game_State = EGame_State::EGS_Play_Level;
   
   for (i = 0; i < 3; ++i)
      Balls[i].Set_State(EBall_State::EBS_On_Platform, Platform.X_Pos + Platform.Width / 2, AsConfig::Start_Ball_Y_Pos);
   
   for (; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Set_State(EBall_State::EBS_Disabled);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Play_Level()
{
   int i;
   int active_balls_count = 0;
   int lost_ball_count = 0;

   for (i = 0; i < AsConfig::Max_Balls_Count; ++i)
   {
      if(Balls[i].Get_State() == EBall_State::EBS_Disabled)
         continue;

      ++active_balls_count;

      if (Balls[i].Get_State() == EBall_State::EBS_Lost)
      {
         ++lost_ball_count;
         continue;
      }

      Balls[i].Move();
   }

   if(active_balls_count == lost_ball_count)
   {// Lost all balls

      Game_State = EGame_State::EGS_Lost_Ball;
      Level.Stop();
      Platform.Set_State(EPlatform_State::EPS_Meltdown);
   }

   if(active_balls_count == 1)
      if (Balls[0].Is_Test_Finished())
         Game_State = EGame_State::EGS_Test_Ball; // Only the ball with index 0 participates in repeated tests
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Act()
{
   int index = 0;
   AFalling_Letter *falling_letter;

   Platform.Act();
   Level.Act();

   while(Level.Get_Next_Falling_Letter(index, &falling_letter))
   {
      if(Platform.Hit_By(falling_letter))
         On_Falling_Letter(falling_letter);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::On_Falling_Letter(AFalling_Letter *falling_letter)
{
   switch (falling_letter->Letter_Type)
   {
   //case ELetter_Type::ELT_O: // "Cancel"
   //   break;
   //case ELetter_Type::ELT_I: // "Inversion"
   //   break;
   //case ELetter_Type::ELT_C: // "Speed"
   //   break;
   //case ELetter_Type::ELT_M: // "Monsters"
   //   break;
   //case ELetter_Type::ELT_G: // "Life"
   //   break;
   //case ELetter_Type::ELT_K: // "Glue"
   //   break;
   //case ELetter_Type::ELT_W: // "Extension"
   //   break;
   //case ELetter_Type::ELT_P: // "Floor"
   //   break;
   //case ELetter_Type::ELT_L: // "Laser"
   //   break;

   case ELetter_Type::ELT_T: // "Three"
      break;

   //case ELetter_Type::ELT_Plus: // "Moving to the next level"
   //   break;

   default:
      AsConfig::Throw();
   }

   falling_letter->Finalize();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
