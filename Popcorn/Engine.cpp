#include "Engine.h"

//--------------AsBall_Set--------------------
void AsBall_Set::Begin_Movement()
{
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Begin_Movement();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Finish_Movement()
{
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Finish_Movement();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Advance(double max_speed)
{
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Advance(max_speed);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

double AsBall_Set::Get_Speed()
{
   double curr_speed, max_speed = 0.0;

   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
   {
      curr_speed = Balls[i].Get_Speed();

      if(curr_speed > max_speed)
         max_speed = curr_speed;
   }

   return max_speed;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Draw(HDC hdc, RECT &paint_area)
{
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Draw(hdc, paint_area);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Release_From_Platform(double platform_x_pos)
{
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      if (Balls[i].Get_State() == EBall_State::EBS_On_Platform)
         Balls[i].Set_State(EBall_State::EBS_Normal, platform_x_pos, AsConfig::Start_Ball_Y_Pos);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Set_On_Platform(double platform_x_pos)
{
   int i;

   for (i = 0; i < 3; ++i)
      Balls[i].Set_State(EBall_State::EBS_On_Platform, platform_x_pos, AsConfig::Start_Ball_Y_Pos);

   for (; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Set_State(EBall_State::EBS_Disabled);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsBall_Set::All_Balls_Are_Lost()
{
   int active_balls_count = 0;
   int lost_ball_count = 0;

   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
   {
      if (Balls[i].Get_State() == EBall_State::EBS_Disabled)
         continue;

      ++active_balls_count;

      if (Balls[i].Get_State() == EBall_State::EBS_Lost)
      {
         ++lost_ball_count;
         continue;
      }
   }

   if (active_balls_count == lost_ball_count)
      return true;
   else
      return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Set_For_Test()
{
   Balls[0].Set_For_Test(); // Only the ball with index 0 participates in repeated tests
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsBall_Set::Is_Test_Finished()
{
   return Balls[0].Is_Test_Finished();  // Only the ball with index 0 participates in repeated tests
}
//-----------------------------------------------------------------------------------------------------------------------------------------------


//--------------AsEngine--------------------
AsEngine::AsEngine()
   : Game_State(EGame_State::EGS_Lost_Ball), Rest_Distance(0)
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
   Advance_Mover();

   if(Ball_Set.All_Balls_Are_Lost())
   {// Lost all balls

      Game_State = EGame_State::EGS_Lost_Ball;
      Level.Stop();
      Platform.Set_State(EPlatform_State::EPS_Meltdown);
   }

   if (Ball_Set.Is_Test_Finished())
      Game_State = EGame_State::EGS_Test_Ball;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Advance_Mover()
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
