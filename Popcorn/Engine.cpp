#include "Engine.h"




//--------------AsEngine--------------------
AsEngine::AsEngine()
   : Game_State(EGame_State::EGS_Lost_Ball), Rest_Distance(0), Life_Count(AsConfig::Initial_Life_Count)
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

   memset(Movers, 0, sizeof(Movers));
   Movers[0] = &Platform;
   Movers[1] = &Ball_Set;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Drawing the game screen

   SetGraphicsMode(hdc, GM_ADVANCED);

   Level.Draw(hdc, paint_area);
   Border.Draw(hdc, paint_area);
   Platform.Draw(hdc, paint_area);
   Ball_Set.Draw(hdc, paint_area);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
   switch (key_type)
   {
   case EKey_Type::EKT_Left:
      Platform.Move(true, key_down);
      break;


   case EKey_Type::EKT_Right:
      Platform.Move(false, key_down);
      break;


   case EKey_Type::EKT_Space:
      if(key_down)
         if(Platform.Get_State() == EPlatform_State::EPS_Ready)
         {
            Ball_Set.Release_From_Platform(Platform.Get_Middle_Pos());

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
      Ball_Set.Set_For_Test();
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
      {
         Game_State = EGame_State::EGS_Play_Level;
         Ball_Set.Set_On_Platform(Platform.Get_Middle_Pos());
      }
      break;
   }

   Act();

   return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Play_Level()
{
   Advance_Movers();

   if(Ball_Set.All_Balls_Are_Lost())
   {// Lost all balls

      Game_State = EGame_State::EGS_Lost_Ball;
      Level.Stop();
      Platform.Set_State(EPlatform_State::EPS_Meltdown);
   }
   else
      Ball_Set.Accelerate();

   if (Ball_Set.Is_Test_Finished())
      Game_State = EGame_State::EGS_Test_Ball;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Advance_Movers()
{
   double curr_speed, max_speed = 0.0;

   // Getting the maximum speed of moving objects
   for (int i = 0; i < AsConfig::Max_Movers_Count; ++i)
   {
      if (Movers[i] != 0)
      {
         Movers[i]->Begin_Movement();

         curr_speed = fabs(Movers[i]->Get_Speed());

         if (curr_speed > max_speed)
            max_speed = curr_speed;
      }
   }

   // Move all moving objects
   Rest_Distance += max_speed;

   while (Rest_Distance > 0.0)
   {
      for (int i = 0; i < AsConfig::Max_Movers_Count; ++i)
         if (Movers[i] != 0)
            Movers[i]->Advance(max_speed);

      Rest_Distance -= AsConfig::Moving_Step_Size;
   }

   // All movements on this frame end
   for (int i = 0; i < AsConfig::Max_Movers_Count; ++i)
      if (Movers[i] != 0)
         Movers[i]->Finish_Movement();
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
   case ELetter_Type::ELT_I: // "Inversion"
      Ball_Set.Inverse_Ball();
      break;
   case ELetter_Type::ELT_C: // "Speed"
      Ball_Set.Reset_Speed();
      break;
   //case ELetter_Type::ELT_M: // "Monsters"
   //   break;
   case ELetter_Type::ELT_G: // "Life"
      if(Life_Count < AsConfig::Max_Life_Count)
         ++Life_Count; // !!! Display on indicator
      break;
   //case ELetter_Type::ELT_K: // "Glue"
   //   break;
   //case ELetter_Type::ELT_W: // "Extension"
   //   break;
   case ELetter_Type::ELT_P: // "Floor"
      AsConfig::Level_Has_Floor = true;
      Border.Redraw_Floor();
      // !!! Display on indicator
      break;
   //case ELetter_Type::ELT_L: // "Laser"
   //   break;

   case ELetter_Type::ELT_T: // "Three"
      Ball_Set.Triple_Balls();
      break;

   //case ELetter_Type::ELT_Plus: // "Moving to the next level"
   //   break;

   default:
      AsConfig::Throw();
   }

   falling_letter->Finalize();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
